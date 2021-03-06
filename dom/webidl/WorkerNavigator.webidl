/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

interface WorkerNavigator {
  [Constant]
  readonly attribute DOMString appName;
  [Constant]
  readonly attribute DOMString appVersion;
  [Constant]
  readonly attribute DOMString platform;
  [Constant]
  readonly attribute DOMString userAgent;
};
