package com.maq.kitkitProvider;

public class Fish {
    public int _id;
    public String _userName;
    public String _fishID;
    public int _skinNo;
    public String _name;
    public String _createTime;
    public String _position;

    @Override
    public String toString() {
        // for json format
        return "{" +
                "\"_id\":" + _id +
                ", \"_userName\":\"" + _userName + "\"" +
                ", \"_fishID\":\"" + _fishID + "\"" +
                ", \"_skinNo\":" + _skinNo +
                ", \"_name\":\"" + _name + "\"" +
                ", \"_createTime\":\"" + _createTime + "\"" +
                ", \"_position\":\"" + _position + "\"" +
                "}";
    }
}
