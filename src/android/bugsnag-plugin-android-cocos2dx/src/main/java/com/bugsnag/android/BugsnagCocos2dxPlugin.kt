package com.bugsnag.android

import com.bugsnag.android.cocos2dx.BuildConfig

public class BugsnagCocos2dxPlugin : BugsnagPlugin {

    override var loaded = false

    companion object {
        @JvmStatic
        fun register(client: Client) {
            BugsnagPluginInterface.registerPlugin(BugsnagCocos2dxPlugin::class.java)
        }
    }

    override fun loadPlugin(client: Client) {
        client.enableNdkCrashReporting()
        client.config.beforeSend { report ->
            report.notifier.name = "Bugsnag Cocos2d-x"
            report.notifier.version = BuildConfig.VERSION_NAME
            report.notifier.setURL("https://github.com/bugsnag/bugsnag-cocos2dx")

            val cocos2dVersion = getCocos2dVersion().split("-", " ").last()
            var runtimeVersions = getRuntimeVersions(report)
            runtimeVersions.set("cocos2dx", cocos2dVersion as Object)
            true
        }
        Logger.info("Initialized Cocos2d-x plugin " + BuildConfig.VERSION_NAME);
        loaded = true
    }

    override fun unloadPlugin() {

    }

    private fun getRuntimeVersions(report: Report): MutableMap<String, Object> {
        val device = report.getError().getDeviceData()
        var runtimeVersions = device.get("runtimeVersions")
        if (runtimeVersions is MutableMap<*, *>) {
            return runtimeVersions as MutableMap<String, Object>
        }
        runtimeVersions = HashMap<String, Object>()
        device.set("runtimeVersions", runtimeVersions)
        return runtimeVersions
    }

    external fun getCocos2dVersion(): String
}
