const std = @import("std");

pub fn build(b: *std.Build) void {
    const exe = b.addExecutable(.{
        .name = "mini-rts-server",
        .optimize = .ReleaseSmall,
        .target = b.standardTargetOptions(.{}),
    });
    exe.addCSourceFile(.{ .file = b.path("src/main.cpp") });
    exe.addCSourceFile(.{ .file = b.path("src/rts/game.cpp") });
    exe.addCSourceFile(.{ .file = b.path("src/rts/board.cpp") });
    exe.addCSourceFile(.{ .file = b.path("src/rts/player.cpp") });
    exe.addCSourceFile(.{ .file = b.path("src/rts/field.cpp") });
    exe.addCSourceFile(.{ .file = b.path("src/rts/unit.cpp") });
    exe.addCSourceFile(.{ .file = b.path("src/net/server.cpp") });
    exe.addCSourceFile(.{ .file = b.path("src/net/client.cpp") });
    exe.addCSourceFile(.{ .file = b.path("src/msg/addressUnit.cpp") });
    exe.addCSourceFile(.{ .file = b.path("src/msg/handler.cpp") });
    exe.addCSourceFile(.{ .file = b.path("src/msg/stringBuffer.cpp") });
    exe.addCSourceFile(.{ .file = b.path("src/msg/unitCommands.cpp") });
    exe.addIncludePath(b.path("src/"));
    exe.linkLibCpp();
    exe.linkLibC();
    b.installArtifact(exe);
}
