// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IOS_CHROME_BROWSER_UI_SETTINGS_REAUTHENTICATION_MODULE_FOR_TESTING_H_
#define IOS_CHROME_BROWSER_UI_SETTINGS_REAUTHENTICATION_MODULE_FOR_TESTING_H_

#import "ios/chrome/browser/ui/settings/reauthentication_module.h"

#import <LocalAuthentication/LocalAuthentication.h>

@interface ReauthenticationModule (ForTesting)

// Allows the replacement of the |LAContext| objects used by
// |ReauthenticationModule| with a mock to facilitate testing.
- (void)setCreateLAContext:(LAContext* (^)(void))createLAContext;

@end

#endif  // IOS_CHROME_BROWSER_UI_SETTINGS_REAUTHENTICATION_MODULE_FOR_TESTING_H
