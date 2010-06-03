/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 * vim: set ts=4 sw=4 et tw=99:
 *
 * ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Mozilla SpiderMonkey JavaScript 1.9 code, released
 * May 28, 2008.
 *
 * The Initial Developer of the Original Code is
 *   Mozilla Foundation
 * Portions created by the Initial Developer are Copyright (C) 2009
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Andreas Gal <gal@mozilla.com>
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either of the GNU General Public License Version 2 or later (the "GPL"),
 * or the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#ifndef jsproxy_h___
#define jsproxy_h___

#include "jsapi.h"
#include "jsobj.h"

namespace js {

/* Base class for all C++ proxy handlers. */
class JSProxyHandler {
  public:
    virtual ~JSProxyHandler();

    /* ES5 Harmony fundamental proxy traps. */
    virtual bool getPropertyDescriptor(JSContext *cx, JSObject *proxy, jsid id,
                                       JSPropertyDescriptor *desc) = 0;
    virtual bool getOwnPropertyDescriptor(JSContext *cx, JSObject *proxy, jsid id,
                                          JSPropertyDescriptor *desc) = 0;
    virtual bool defineProperty(JSContext *cx, JSObject *proxy, jsid id,
                                JSPropertyDescriptor *desc) = 0;
    virtual bool getOwnPropertyNames(JSContext *cx, JSObject *proxy, JSIdArray **idap) = 0;
    virtual bool delete_(JSContext *cx, JSObject *proxy, jsid id, bool *bp) = 0;
    virtual bool enumerate(JSContext *cx, JSObject *proxy, JSIdArray **idap) = 0;
    virtual bool fix(JSContext *cx, JSObject *proxy, jsval *vp) = 0;

    /* ES5 Harmony derived proxy traps. */
    virtual bool has(JSContext *cx, JSObject *proxy, jsid id, bool *bp);
    virtual bool hasOwn(JSContext *cx, JSObject *proxy, jsid id, bool *bp);
    virtual bool get(JSContext *cx, JSObject *proxy, JSObject *receiver, jsid id, jsval *vp);
    virtual bool set(JSContext *cx, JSObject *proxy, JSObject *receiver, jsid id, jsval *vp);
    virtual bool enumerateOwn(JSContext *cx, JSObject *proxy, JSIdArray **idap);
    virtual bool iterate(JSContext *cx, JSObject *proxy, uintN flags, jsval *vp);

    /* Spidermonkey extensions. */
    virtual void finalize(JSContext *cx, JSObject *proxy);
    virtual void trace(JSTracer *trc, JSObject *proxy);
    virtual const void *family() = 0;
};

/* Dispatch point for handlers that executes the appropriate C++ or scripted traps. */
class JSProxy {
  public:
    /* ES5 Harmony fundamental proxy traps. */
    static bool getPropertyDescriptor(JSContext *cx, JSObject *proxy, jsid id,
                                      JSPropertyDescriptor *desc);
    static bool getPropertyDescriptor(JSContext *cx, JSObject *proxy, jsid id, jsval *vp);
    static bool getOwnPropertyDescriptor(JSContext *cx, JSObject *proxy, jsid id,
                                         JSPropertyDescriptor *desc);
    static bool getOwnPropertyDescriptor(JSContext *cx, JSObject *proxy, jsid id, jsval *vp);
    static bool defineProperty(JSContext *cx, JSObject *proxy, jsid id, JSPropertyDescriptor *desc);
    static bool defineProperty(JSContext *cx, JSObject *proxy, jsid id, jsval v);
    static bool getOwnPropertyNames(JSContext *cx, JSObject *proxy, JSIdArray **idap);
    static bool delete_(JSContext *cx, JSObject *proxy, jsid id, bool *bp);
    static bool enumerate(JSContext *cx, JSObject *proxy, JSIdArray **idap);
    static bool fix(JSContext *cx, JSObject *proxy, jsval *vp);

    /* ES5 Harmony derived proxy traps. */
    static bool has(JSContext *cx, JSObject *proxy, jsid id, bool *bp);
    static bool hasOwn(JSContext *cx, JSObject *proxy, jsid id, bool *bp);
    static bool get(JSContext *cx, JSObject *proxy, JSObject *receiver, jsid id, jsval *vp);
    static bool set(JSContext *cx, JSObject *proxy, JSObject *receiver, jsid id, jsval *vp);
    static bool enumerateOwn(JSContext *cx, JSObject *proxy, JSIdArray **idap);
    static bool iterate(JSContext *cx, JSObject *proxy, uintN flags, jsval *vp);
};

/* Shared between object and function proxies. */
const uint32 JSSLOT_PROXY_HANDLER = JSSLOT_PRIVATE + 0;
/* Object proxies only. */
const uint32 JSSLOT_PROXY_CLASS = JSSLOT_PRIVATE + 1;
const uint32 JSSLOT_PROXY_PRIVATE = JSSLOT_PRIVATE + 2;
/* Function proxies only. */
const uint32 JSSLOT_PROXY_CALL = JSSLOT_PRIVATE + 1;
const uint32 JSSLOT_PROXY_CONSTRUCT = JSSLOT_PRIVATE + 2;

extern JS_FRIEND_API(JSClass) ObjectProxyClass;
extern JS_FRIEND_API(JSClass) FunctionProxyClass;
extern JSClass CallableObjectClass;

}

inline bool
JSObject::isObjectProxy() const
{
    return getClass() == &js::ObjectProxyClass;
}

inline bool
JSObject::isFunctionProxy() const
{
    return getClass() == &js::FunctionProxyClass;
}

inline bool
JSObject::isProxy() const
{
    return isObjectProxy() || isFunctionProxy();
}

inline jsval
JSObject::getProxyHandler() const
{
    JS_ASSERT(isProxy());
    jsval handler = fslots[js::JSSLOT_PROXY_HANDLER];
    JS_ASSERT(JSVAL_IS_OBJECT(handler) || JSVAL_IS_INT(handler));
    return handler;
}

inline jsval
JSObject::getProxyPrivate() const
{
    JS_ASSERT(isObjectProxy());
    return fslots[js::JSSLOT_PROXY_PRIVATE];
}

inline void
JSObject::setProxyPrivate(jsval priv)
{
    JS_ASSERT(isObjectProxy());
    fslots[js::JSSLOT_PROXY_PRIVATE] = priv;
}

namespace js {

JS_FRIEND_API(JSObject *)
NewObjectProxy(JSContext *cx, jsval handler, JSObject *proto, JSObject *parent,
               JSString *className);

JS_FRIEND_API(JSObject *)
NewFunctionProxy(JSContext *cx, jsval handler, JSObject *proto, JSObject *parent,
                 JSObject *call, JSObject *construct);

JS_FRIEND_API(JSBool)
GetProxyObjectClass(JSContext *cx, JSObject *proxy, const char **namep);

JS_FRIEND_API(JSBool)
FixProxy(JSContext *cx, JSObject *proxy, JSBool *bp);

}

JS_BEGIN_EXTERN_C

extern JSClass js_ProxyClass;

extern JS_FRIEND_API(JSObject *)
js_InitProxyClass(JSContext *cx, JSObject *obj);

JS_END_EXTERN_C

#endif
