use std::env;
// use std::process::Command;

fn main() {
    let out_dir = env::var("OUT_DIR").unwrap();

    // Command::new("x86_64-elf-gcc")
    //     .args(&["src/startup.c", "-c", "-march=i386", "-m16", "-ffreestanding", "-fno-pie", "-o"])
    //     .arg(&format!("{}/startup.o", out_dir))
    //     .status().unwrap();

    println!("cargo:rustc-link-search=native={}", out_dir);
}