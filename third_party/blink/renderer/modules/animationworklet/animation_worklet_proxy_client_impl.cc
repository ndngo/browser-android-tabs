// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/modules/animationworklet/animation_worklet_proxy_client_impl.h"

#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/frame/web_frame_widget_base.h"
#include "third_party/blink/renderer/core/frame/web_local_frame_impl.h"
#include "third_party/blink/renderer/core/workers/worker_thread.h"
#include "third_party/blink/renderer/platform/cross_thread_functional.h"
#include "third_party/blink/renderer/platform/graphics/compositor_mutator_impl.h"

namespace blink {

AnimationWorkletProxyClientImpl::AnimationWorkletProxyClientImpl(
    int scope_id,
    base::WeakPtr<CompositorMutatorImpl> mutator,
    scoped_refptr<base::SingleThreadTaskRunner> mutator_runner)
    : scope_id_(scope_id),
      mutator_(std::move(mutator)),
      mutator_runner_(std::move(mutator_runner)),
      state_(RunState::kUninitialized) {
  DCHECK(IsMainThread());
}

void AnimationWorkletProxyClientImpl::Trace(blink::Visitor* visitor) {
  AnimationWorkletProxyClient::Trace(visitor);
  CompositorAnimator::Trace(visitor);
}

void AnimationWorkletProxyClientImpl::SetGlobalScope(
    WorkletGlobalScope* global_scope) {
  DCHECK(global_scope);
  DCHECK(global_scope->IsContextThread());
  if (state_ == RunState::kDisposed)
    return;
  DCHECK(state_ == RunState::kUninitialized);

  global_scope_ = static_cast<AnimationWorkletGlobalScope*>(global_scope);
  // TODO(majidvp): Add an AnimationWorklet task type when the spec is final.
  scoped_refptr<base::SingleThreadTaskRunner> global_scope_runner =
      global_scope_->GetThread()->GetTaskRunner(TaskType::kMiscPlatformAPI);
  state_ = RunState::kWorking;
  DCHECK(mutator_runner_);
  PostCrossThreadTask(
      *mutator_runner_, FROM_HERE,
      CrossThreadBind(&CompositorMutatorImpl::RegisterCompositorAnimator,
                      mutator_, WrapCrossThreadPersistent(this),
                      global_scope_runner));
}

void AnimationWorkletProxyClientImpl::Dispose() {
  if (state_ == RunState::kWorking) {
    // At worklet scope termination break the reference to the Client from
    // the compositor if it is still alive.
    DCHECK(mutator_runner_);
    PostCrossThreadTask(
        *mutator_runner_, FROM_HERE,
        CrossThreadBind(&CompositorMutatorImpl::UnregisterCompositorAnimator,
                        mutator_, WrapCrossThreadPersistent(this)));

    DCHECK(global_scope_);
    DCHECK(global_scope_->IsContextThread());

    // At worklet scope termination break the reference cycle between
    // AnimationWorkletGlobalScope and AnimationWorkletProxyClientImpl.
    global_scope_ = nullptr;
  }

  mutator_runner_ = nullptr;
  DCHECK(state_ != RunState::kDisposed);
  state_ = RunState::kDisposed;
}

std::unique_ptr<AnimationWorkletOutput> AnimationWorkletProxyClientImpl::Mutate(
    std::unique_ptr<AnimationWorkletInput> input) {
  DCHECK(input);
#if DCHECK_IS_ON()
  DCHECK(input->ValidateScope(scope_id_))
      << "Input has state that does not belong to this global scope: "
      << scope_id_;
#endif

  if (!global_scope_)
    return nullptr;

  auto output = global_scope_->Mutate(*input);

  // TODO(petermayo): https://crbug.com/791280 PostCrossThreadTask to supply
  // this rather than return it.
  return output;
}

// static
AnimationWorkletProxyClientImpl* AnimationWorkletProxyClientImpl::FromDocument(
    Document* document,
    int scope_id) {
  WebLocalFrameImpl* local_frame =
      WebLocalFrameImpl::FromFrame(document->GetFrame());
  scoped_refptr<base::SingleThreadTaskRunner> mutator_queue;
  base::WeakPtr<CompositorMutatorImpl> mutator =
      local_frame->LocalRootFrameWidget()->EnsureCompositorMutator(
          &mutator_queue);
  return new AnimationWorkletProxyClientImpl(scope_id, std::move(mutator),
                                             std::move(mutator_queue));
}

}  // namespace blink
