const std = @import("std");

pub fn build(b: *std.Build) !void {
    var sources = std.ArrayList([]const u8).init(b.allocator);
    defer sources.deinit();
    defer for (sources.items) |source| {
        b.allocator.free(source);
    };

    var dir = try std.fs.cwd().openDir("src/", .{ .iterate = true });
    const allowed_exts = [_][]const u8{".cpp"};

    var walker = try dir.walk(b.allocator);
    defer walker.deinit();

    while (try walker.next()) |entry| {
        const ext = std.fs.path.extension(entry.basename);
        const include_file = for (allowed_exts) |e| {
            if (std.mem.eql(u8, ext, e))
                break true;
        } else false;

        if (include_file) {
            const ptr = try b.allocator.alloc(u8, entry.path.len + 4);
            @memcpy(ptr[0..4], "src/");
            @memcpy(ptr[4..], entry.path);
            try sources.append(ptr);
        }
    }

    const exe = b.addExecutable(.{
        .name = "mini-rts-server",
        .optimize = .ReleaseSmall,
        .target = b.standardTargetOptions(.{}),
    });
    exe.addCSourceFiles(.{ .files = sources.items });
    exe.addIncludePath(b.path("src/"));
    exe.linkLibCpp();
    exe.linkLibC();
    b.installArtifact(exe);
}
