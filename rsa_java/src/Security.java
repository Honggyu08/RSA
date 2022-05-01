import javax.crypto.*;
import java.io.UnsupportedEncodingException;
import java.security.*;
import java.security.spec.InvalidKeySpecException;

/**
 * Security-related methods. For a secure implementation, all of this code
 * should be implemented on a server that communicates with the
 * application on the device. For the sake of simplicity and clarity of this
 * example, this code is included here and is executed on the device. If you
 * must verify the purchases on the phone, you should obfuscate this code to
 * make it harder for an attacker to replace the code with stubs that treat all
 * purchases as verified.
 */

public class Security {
    private static final int DEFAULT_KEY_SIZE = 2048;

    private static final String KEY_FACTORY_ALGORITHM = "RSA";

    private static final String SIGNATURE_ALGORITHM = "SHA256withRSA";

    private static final String CHARSET = "UTF-8";

    /**
     * Generate key pair that public and private key by RSA
     * @return key pair
     */
    public static KeyPair generateKeyPair() throws NoSuchAlgorithmException {
        KeyPairGenerator generator = KeyPairGenerator.getInstance(KEY_FACTORY_ALGORITHM);
        generator.initialize(DEFAULT_KEY_SIZE, new SecureRandom());
        KeyPair pair = generator.generateKeyPair();
        return pair;
    }

    /**
     * Encrypt plain text by RSA algorithm
     * @param plainText
     * @param keyPub
     * @return cipher text
     * @throws NoSuchAlgorithmException
     */
    public static byte[] encrypt(String plainText, PublicKey keyPub) throws NoSuchAlgorithmException {
        PublicKey publicKey = keyPub;
        try {
            Cipher cipher = Cipher.getInstance(KEY_FACTORY_ALGORITHM);
            cipher.init(Cipher.ENCRYPT_MODE, publicKey);
            byte[] bytes = cipher.doFinal(plainText.getBytes(CHARSET));
            //return Base64.getEncoder().encodeToString(bytes);
            return bytes;
        } catch (NoSuchPaddingException | InvalidKeyException | UnsupportedEncodingException | IllegalBlockSizeException | BadPaddingException e) {
            throw new RuntimeException(e);
        }
    }

    /**
     * Decrypt cipher text by RSA algorithm
     * @param cipherText
     * @param keyPriv
     * @return plain text
     * @throws NoSuchAlgorithmException
     */
    public static byte[] decrypt(byte[] cipherText, PrivateKey keyPriv) throws NoSuchAlgorithmException {
        PrivateKey privateKey = keyPriv;
        try {
            byte[] bytes = cipherText/**.getBytes()*/;
            Cipher cipher = Cipher.getInstance(KEY_FACTORY_ALGORITHM);
            cipher.init(Cipher.DECRYPT_MODE, privateKey);
            return cipher.doFinal(bytes);
        } catch (NoSuchPaddingException | InvalidKeyException | IllegalBlockSizeException | BadPaddingException e) {
            throw new RuntimeException(e);
        }
    }

    /**
     * Create signature by signing
     * @param plainText the signed JSON string (signed, not encrypted)
     * @param encodedPrivateKey the base64-encoded private key to use for signing.
     * @return signature text
     *//**
     public static String sign(String plainText, byte[] encodedPrivateKey) {
     try {
     Signature privateSignature = Signature.getInstance(SIGNATURE_ALGORITHM);
     privateSignature.initSign(Security.generatePrivateKey(encodedPrivateKey));
     privateSignature.update(plainText.getBytes(CHARSET));
     byte[] signature = privateSignature.sign();
     return Base64.getEncoder().encodeToString(signature);
     } catch (NoSuchAlgorithmException | InvalidKeyException | UnsupportedEncodingException | SignatureException e) {
     throw new RuntimeException(e);
     }
     }*/

    /**
     * Verifies that the data was signed with the given signature, and returns
     * the verified purchase. The data is in JSON format and signed
     * and product ID of the purchase.
     * @param plainText the signed JSON string (signed, not encrypted)
     * @param signature the signature for the data, signed with the private key
     * @param encodedPublicKey the base64-encoded public key to use for verifying.
     * @return result for verification
     *//**
     public static boolean verify(String plainText, String signature, byte[] encodedPublicKey) {
     PublicKey publicKey = Security.generatePublicKey(encodedPublicKey);
     return Security.verifySignarue(plainText, signature, publicKey);
     }*/
/**
    private static PublicKey generatePublicKey(byte[] encodedPublicKey) {
        try {
            KeyFactory keyFactory = KeyFactory.getInstance(KEY_FACTORY_ALGORITHM);
            return keyFactory.generatePublic(new X509EncodedKeySpec(encodedPublicKey));
        } catch (NoSuchAlgorithmException e) {
            throw new RuntimeException(e);
        } catch (InvalidKeySpecException e) {
            throw new IllegalArgumentException(e);
        }
    }

    private static PrivateKey generatePrivateKey(byte[] encodedPrivateKey) {
        try {
            KeyFactory keyFactory = KeyFactory.getInstance(KEY_FACTORY_ALGORITHM);
            return keyFactory.generatePrivate(new PKCS8EncodedKeySpec(encodedPrivateKey));
        } catch (NoSuchAlgorithmException e) {
            throw new RuntimeException(e);
        } catch (InvalidKeySpecException e) {
            throw new IllegalArgumentException(e);
        }
    }*/
    /**
     private static boolean verifySignarue(String plainText, String signature, PublicKey publicKey) {
     Signature sig;
     try {
     sig = Signature.getInstance(SIGNATURE_ALGORITHM);
     sig.initVerify(publicKey);
     sig.update(plainText.getBytes());
     if (!sig.verify(Base64.getDecoder().decode(signature)))
     throw new InvalidSignatureException("It was awesome! Signature hasn't be invalid");
     } catch (NoSuchAlgorithmException | InvalidKeyException | SignatureException e) {
     throw new RuntimeException(e);
     }
     return true;
     }*/

    public static String byteArrayToHexaString(byte[] bytes) {
        StringBuilder builder = new StringBuilder();

        for (byte data : bytes) {
            builder.append(String.format("%02X", data));
        }
        return builder.toString();
    }
}

class InvalidSignatureException extends RuntimeException {

    InvalidSignatureException(String message) {
        super(message);
    }
}