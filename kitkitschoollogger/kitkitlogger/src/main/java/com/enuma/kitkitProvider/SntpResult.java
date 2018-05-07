package com.enuma.kitkitProvider;

// NB(xenosoz, 2018): Created. 2018-dapr-26.

public class SntpResult {
    public String serverSpec;
    public long now;
    public String snow;

    public SntpResult(String serverSpec, long now, String snow) {
        this.serverSpec = serverSpec;
        this.now = now;
        this.snow = snow;
    }
}
