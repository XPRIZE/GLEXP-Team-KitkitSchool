package com.maq.kitkitProvider;

// NB(xenosoz, 2018): Created. 2018-dapr-26.

public class SntpResult {
    public int id;
    public String serverSpec;
    public long now;
    public String snow;

    public SntpResult(String serverSpec, long now, String snow) {
        init(-1, serverSpec, now, snow);
    }

    public SntpResult(int id, String serverSpec, long now, String snow) {
        init(id, serverSpec, now, snow);
    }

    private void init(int id, String serverSpec, long now, String snow) {
        this.id = id;
        this.serverSpec = serverSpec;
        this.now = now;
        this.snow = snow;
    }
}
