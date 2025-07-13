plugins {
    alias(libs.plugins.android.application)
}

android {
    namespace = "ss.colytitse.amatsutsumi"
    compileSdk = 35


    defaultConfig {

        applicationId = "ss.colytitse.amatsutsumi"
        minSdk = 24
        targetSdk = 35
        versionCode = 1
        versionName = "1.0"

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
        ndk {
            abiFilters += listOf("arm64-v8a", "armeabi-v7a", "x86", "x86_64")
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
    }

    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "3.22.1"
        }
    }

    packaging {
        jniLibs {
            excludes += listOf("lib/*/libdobby.so")
        }
    }

    buildFeatures {
        prefab = true
    }

}

dependencies {
    testImplementation(libs.junit)
    androidTestImplementation(libs.ext.junit)
}