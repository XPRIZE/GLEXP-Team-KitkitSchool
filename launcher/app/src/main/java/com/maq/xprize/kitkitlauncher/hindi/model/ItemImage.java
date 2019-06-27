package com.maq.xprize.kitkitlauncher.hindi.model;

import android.os.Parcel;
import android.os.Parcelable;

public class ItemImage implements Parcelable {

    public String mImagePath = "";
    public String mThumbnailPath = "";
    public long mLastModified = 0L;

    public ItemImage() {
    }

    public ItemImage(Parcel in) {
        readFromParcel(in);
    }

    @Override
    public int describeContents() {
        return 0;
    }

    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(mImagePath);
        dest.writeString(mThumbnailPath);
        dest.writeLong(mLastModified);
    }

    private void readFromParcel(Parcel in){
        mImagePath = in.readString();
        mThumbnailPath = in.readString();
        mLastModified = in.readLong();
    }

    public static final Creator CREATOR = new Creator() {
        public ItemImage createFromParcel(Parcel in) {
            return new ItemImage(in);
        }

        public ItemImage[] newArray(int size) {
            return new ItemImage[size];
        }
    };

    @Override
    public String toString() {
        return "{" +
                "\"mImagePath\":\"" + mImagePath + "\"" +
                ", \"mThumbnailPath\":\"" + mThumbnailPath + "\"" +
                ", \"mLastModified\":\"" + mLastModified + "\"" +
                "}";
    }
}
