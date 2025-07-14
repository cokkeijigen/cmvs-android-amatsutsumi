package ss.colytitse.amatsutsumi;

@SuppressWarnings("unused")
public class Amatsutsumi {

    static {
        System.loadLibrary("amatsutsumihook");
    }

    /**
     * A native method that is implemented by the 'amatsutsumihook' native library,
     * which is packaged with this application.
     */
    public static native String stringFromJNI();
}
