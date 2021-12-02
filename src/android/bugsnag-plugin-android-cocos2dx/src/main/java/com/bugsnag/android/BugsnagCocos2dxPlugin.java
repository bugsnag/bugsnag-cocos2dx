package com.bugsnag.android;

import java.util.HashMap;
import java.util.Map;

public class BugsnagCocos2dxPlugin implements BugsnagPlugin {

    /** Formatted, cached copy of the cocos2d version */
    private static String version = null;

    /** Release version of the bugsnag cocos2d-x plugin */
    private static String pluginVersion = "1.0.0";

    private boolean loaded = false;

    /**
     * Register the plugin as available when Bugsnag initializes. Must be run prior to
     * Bugsnag.init() or Client constructor.
     */
    public static void register() {
        BugsnagPluginInterface.INSTANCE.registerPlugin(BugsnagCocos2dxPlugin.class);  
    } 

    static {
        String versionComponents[] = getCocos2dVersion().split("[\\-\\s]");
        version = versionComponents[versionComponents.length - 1];
    }

    @Override
    public void setLoaded(boolean loaded) {
        this.loaded = loaded;
    }

    @Override
    public boolean getLoaded() {
        return loaded;
    }

    @Override
    public void unloadPlugin() {
    }

    @Override
    public void loadPlugin(Client client) {
        configureNotifierDetails();
        configureNativeComponents();
        client.enableNdkCrashReporting();
        client.getConfig().beforeSend(new BeforeSend() {
            @Override
            public boolean run(Report report) {
                Map<String, Object> runtimeVersions = 
                    getRuntimeVersions(report.getError().getDeviceData());
                runtimeVersions.put("cocos2dx", version);
            return true;
            }
        });
        client.getConfig().addBeforeSendSession(new BeforeSendSession() {
            @Override
            public void beforeSendSession(SessionTrackingPayload payload) {
                Map<String, Object> runtimeVersions = getRuntimeVersions(payload.getDevice());
                runtimeVersions.put("cocos2dx", version);
            }
        });

        Logger.info("Initialized Cocos2d-x plugin " + pluginVersion);
    }

    private void configureNotifierDetails() {
        Notifier notifier = Notifier.getInstance();
        notifier.setName("Bugsnag Cocos2d-x");
        notifier.setVersion(pluginVersion);
        notifier.setURL("https://github.com/bugsnag/bugsnag-cocos2dx");
    }

    private Map<String, Object>getRuntimeVersions(Map<String, Object> device) {
        Object runtimeVersions = device.get("runtimeVersions");
        if (runtimeVersions instanceof Map) {
            @SuppressWarnings("unchecked")
            Map<String, Object> versions = (Map<String, Object>)runtimeVersions;
            return versions;
        }
        Map<String, Object> versions = new HashMap<String, Object>();
        device.put("runtimeVersions", versions);
        return versions;
    }

    private static native String getCocos2dVersion();

    private static native void configureNativeComponents();
}
