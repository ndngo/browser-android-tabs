// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IOS_CHROME_BROWSER_UI_AUTHENTICATION_SIGNIN_EARLGREY_UTILS_H_
#define IOS_CHROME_BROWSER_UI_AUTHENTICATION_SIGNIN_EARLGREY_UTILS_H_

#import <Foundation/Foundation.h>

@class ChromeIdentity;

// Methods used for the EarlGrey tests.
@interface SigninEarlGreyUtils : NSObject

// Returns a fake identity.
+ (ChromeIdentity*)fakeIdentity1;

// Returns a second fake identity.
+ (ChromeIdentity*)fakeIdentity2;

// Returns a fake managed identity.
+ (ChromeIdentity*)fakeManagedIdentity;

// Asserts that |identity| is actually signed in to the active profile.
+ (void)assertSignedInWithIdentity:(ChromeIdentity*)identity;

// Asserts that no identity is signed in.
+ (void)assertSignedOut;

@end

#endif  // IOS_CHROME_BROWSER_UI_AUTHENTICATION_SIGNIN_EARLGREY_UTILS_H_
