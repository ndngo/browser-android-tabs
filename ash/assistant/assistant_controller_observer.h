// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_ASSISTANT_ASSISTANT_CONTROLLER_OBSERVER_H_
#define ASH_ASSISTANT_ASSISTANT_CONTROLLER_OBSERVER_H_

#include <map>
#include <string>

#include "base/macros.h"

class GURL;

namespace ash {

namespace assistant {
namespace util {
enum class DeepLinkType;
}  // namespace util
}  // namespace assistant

class AssistantControllerObserver {
 public:
  // Invoked when the AssistantController has been fully constructed.
  virtual void OnAssistantControllerConstructed() {}

  // Invoked when the AssistantController is starting to be destroyed.
  virtual void OnAssistantControllerDestroying() {}

  // Invoked when Assistant has received a deep link of the specified |type|
  // with the given |params|.
  virtual void OnDeepLinkReceived(
      assistant::util::DeepLinkType type,
      const std::map<std::string, std::string>& params) {}

  // Invoked when the specified |url| is opened by Assistant in a new tab.
  virtual void OnUrlOpened(const GURL& url) {}

 protected:
  AssistantControllerObserver() = default;
  virtual ~AssistantControllerObserver() = default;

  DISALLOW_COPY_AND_ASSIGN(AssistantControllerObserver);
};

}  // namespace ash

#endif  // ASH_ASSISTANT_ASSISTANT_CONTROLLER_OBSERVER_H_
