package com.bugsnag.android;

import java.util.Arrays;

public class BugsnagCocos2dxPlugin implements Plugin {
    static final String pluginVersion = "1.1.0";

    @Override
    public void unload() {
    }

    @Override
    public void load(Client client) {
        configureNativeComponents();

        Notifier notifier = client.getNotifier();
        notifier.setName("Bugsnag Cocos2d-x");
        notifier.setVersion(pluginVersion);
        notifier.setUrl("https://github.com/bugsnag/bugsnag-cocos2dx");
        notifier.setDependencies(Collections.singletonList(new Notifier())); // depend on bugsnag-android

        String[] versionComponents = getCocos2dVersion().split("[\\-\\s]");
        String version = versionComponents[versionComponents.length - 1];
        client.addRuntimeVersionInfo("cocos2dx", version);

        client.logger.i("Initialized Cocos2d-x Plugin");
    }

    private static native String getCocos2dVersion();

    private static native void configureNativeComponents();
}

