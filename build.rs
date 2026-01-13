fn main() {
    cc::Build::new()
        .cpp(true)
        .file("cpp/engine.cpp")
        .compile("browser_engine");
    
    println!("cargo:rerun-if-changed=cpp/engine.cpp");
    println!("cargo:rerun-if-changed=cpp/engine.hpp");
}
