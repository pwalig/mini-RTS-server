const std = @import("std");

pub fn getCpu(name: []const u8) ?std.Target.Cpu.Arch {
    if (std.mem.eql(u8, name, "x86_64")) return .x86_64;
    if (std.mem.eql(u8, name, "aarch64")) return .aarch64;
    if (std.mem.eql(u8, name, "arm")) return .arm;
    return null;
}

pub fn build(b: *std.Build) void {
    const cpu = b.option([]const u8, "cpu", "select cpu architecture");
    const exe = b.addExecutable(.{
        .name = "mini-rts-server",
        //.root_source_file = b.path("src/main.cpp"),
        .optimize = .ReleaseSmall,
        .target = b.resolveTargetQuery(.{
            .cpu_arch = if (cpu) |cpuName| getCpu(cpuName) else null,
        }),
    });
    exe.addCSourceFile(.{ .file = b.path("src/main.cpp") });
    exe.addIncludePath(b.path("src/"));
    exe.linkLibCpp();
    exe.linkLibC();
    b.installArtifact(exe);
}
