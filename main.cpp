#include "jsapi.h"

/* The class of the global object. */
static JSClass global_class = {
        "global",
        JSCLASS_GLOBAL_FLAGS,
        JS_PropertyStub,
        JS_DeletePropertyStub,
        JS_PropertyStub,
        JS_StrictPropertyStub,
        JS_EnumerateStub,
        JS_ResolveStub,
        JS_ConvertStub,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        JS_GlobalObjectTraceHook
};

int main(int argc, const char *argv[]) {
    JS_Init();

    JSRuntime *rt = JS_NewRuntime(8L * 1024 * 1024);
    if (!rt)
        return 1;

    JSContext *cx = JS_NewContext(rt, 8192);
    if (!cx)
        return 1;

    { // Scope for our various stack objects (JSAutoRequest, RootedObject), so they all go
        // out of scope before we JS_DestroyContext.

        JSAutoRequest ar(cx); // In practice, you would want to exit this any
        // time you're spinning the event loop

        JS::RootedObject global(cx, JS_NewGlobalObject(cx, &global_class, nullptr, JS::FireOnNewGlobalHook));
        if (!global)
            return 1;

        JS::RootedValue rval(cx);

        { // Scope for JSAutoCompartment
            JSAutoCompartment ac(cx, global);
            JS_InitStandardClasses(cx, global);

            const char *script = "'hello'+'world, it is '+new Date()";
            const char *filename = "noname";
            int lineno = 1;
            bool ok = JS_EvaluateScript(cx, global, script, strlen(script), filename, lineno, &rval);
            if (!ok)
                return 1;
        }

        JSString *str = rval.toString();
        printf("%s\n", JS_EncodeString(cx, str));
    }

    JS_DestroyContext(cx);
    JS_DestroyRuntime(rt);
    JS_ShutDown();
    return 0;
}
