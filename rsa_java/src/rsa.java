import java.security.*;

public class rsa {
    public static void main(String[] args) throws Exception {
        KeyPair keyPair = Security.generateKeyPair();

        PublicKey keyPub = keyPair.getPublic();
        PrivateKey keyPriv = keyPair.getPrivate();
        // print key value
        System.out.println(keyPub);
        System.out.println(keyPriv);

        String plainText = "hello world";
        System.out.println("Plain text: " + plainText);

        byte[] encrypted = Security.encrypt(plainText, keyPub);
        String encryptedStr = new String(encrypted, "UTF-8");
        String encryptedNum = Security.byteArrayToHexaString(encrypted);
        System.out.println("Encrypted Number: "+ encryptedNum);
        System.out.println("Encrypted text: " + encryptedStr);

        byte[] decrypted = Security.decrypt(encrypted, keyPriv);
        String decryptedStr = new String(decrypted, "UTF-8");
        String decryptedNum = Security.byteArrayToHexaString(decrypted);
        System.out.println("Decrypted Number: " + decryptedNum);
        System.out.println("Decrypted text: " + decryptedStr);
    }
}
