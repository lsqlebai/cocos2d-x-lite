package com.iflytek.utils.compression;

import com.iflytek.log.Logger;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.util.zip.GZIPInputStream;
import java.util.zip.GZIPOutputStream;

public class GZIPUtil {
    /**
     * Decompress GZip Data
     * @param inputData
     * @return
     */
    public static byte[] decompressGZIP(byte[] inputData){
        if (inputData == null || inputData.length == 0) {
            return new byte[0];
        }
        ByteArrayOutputStream tempBaos = new ByteArrayOutputStream();
        GZIPInputStream gzip = null;
        try {
            gzip = new GZIPInputStream(new ByteArrayInputStream(inputData));
            byte[] buffer = new byte[100 * 1024];
            int length = 0;
            while((length = gzip.read(buffer)) != -1){
                tempBaos.write(buffer, 0, length);
            }
            return tempBaos.toByteArray();
        } catch (Exception e) {
            e.printStackTrace();
            return new byte[0];
        } finally{
            try {
                if(gzip != null){
                    gzip.close();
                }
                if(tempBaos != null){
                    tempBaos.close();
                }
            } catch (IOException ignored) {}
        }
    }

    public static byte[] compressGZIP(byte[] inputData) {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        GZIPOutputStream gzip = null;
        try {
            gzip = new GZIPOutputStream(baos);
            gzip.write(inputData, 0, inputData.length);
            gzip.finish();
            return baos.toByteArray();
        }
        catch (IOException e) {
            Logger.log().e("compressGZIP failed!", e);
            return new byte[0];
        } finally{
            try {
                if (gzip != null) {
                    gzip.close();
                }
                if (baos != null) {
                    baos.close();
                }
            }
            catch (IOException ignored) { }
        }
    }

}
