package com.iflytek.utils.encrypt;

import javax.crypto.Cipher;
import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.DESedeKeySpec;
import java.security.Key;

/**
 * Created by cllu on 14/11/13.
 */
public class DESede {
    private final static String encoding = "utf-8";

    /**
     * ECB加密,不要IV
     *
     * @param _key  密钥
     * @param _data 明文
     * @return Base64编码的密文
     */
    public static byte[] des3EncodeECB(String _key, String _data) {
        try {
            byte[] key = _key.getBytes(encoding);
            byte[] data = _data.getBytes(encoding);
            Key desKey;
            DESedeKeySpec spec = new DESedeKeySpec(key);
            SecretKeyFactory keyFactory = SecretKeyFactory.getInstance("desede");
            desKey = keyFactory.generateSecret(spec);
            Cipher cipher = Cipher.getInstance("desede" + "/ECB/PKCS5Padding");
            cipher.init(Cipher.ENCRYPT_MODE, desKey);
            return cipher.doFinal(data);
        } catch (Exception ex) {
            return null;
        }

    }

    /**
     * ECB解密,不要IV
     *
     * @param _key 密钥
     * @param data Base64编码的密文
     * @return 明文
     */
    public static byte[] des3DecodeECB(String _key, byte[] data) {
        try {
            byte[] key = _key.getBytes(encoding);
            Key desKey;
            DESedeKeySpec spec = new DESedeKeySpec(key);
            SecretKeyFactory keyFactory = SecretKeyFactory.getInstance("desede");
            desKey = keyFactory.generateSecret(spec);
            Cipher cipher = Cipher.getInstance("desede" + "/ECB/PKCS5Padding");
            cipher.init(Cipher.DECRYPT_MODE, desKey);
            return cipher.doFinal(data);
        } catch (Exception ex) {
            return null;
        }
    }
}
