package com.bugsnag.android;

import java.util.HashMap;
import java.util.Map;

import com.bugsnag.android.cocos2dx.BuildConfig;

public class BugsnagCocos2dxPlugin implements BugsnagPlugin {

    /** Formatted, cached copy of the cocos2d version */
    private static String version = null;

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
        configureNativeComponents();
        client.enableNdkCrashReporting();
        client.getConfig().beforeSend(new BeforeSend() {
            @Override
            public boolean run(Report report) {
                report.getNotifier().setName("Bugsnag Cocos2d-x");
                report.getNotifier().setVersion(BuildConfig.VERSION_NAME);
                report.getNotifier().setURL("https://github.com/bugsnag/bugsnag-cocos2dx");

                Map<String, Object> runtimeVersions = getRuntimeVersions(report);
                runtimeVersions.put("cocos2dx", version);
            return true;
            }
        });
        Logger.info("Initialized Cocos2d-x plugin " + BuildConfig.VERSION_NAME);
    }

    private Map<String, Object>getRuntimeVersions(Report report) {
        Map<String, Object> device = report.getError().getDeviceData();
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
