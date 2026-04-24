for manifest in $(find android/ijkplayer -name "AndroidManifest.xml"); do
    pkg=$(grep "package=" "$manifest" | awk -F'"' '{print $2}')
    if [ ! -z "$pkg" ]; then
        echo "Updating $manifest : $pkg"
        
        # 移除 package 属性
        sed -i '' 's/package="[^"]*"//g' "$manifest"
        
        # 查找对应的 build.gradle
        dir=$(dirname "$manifest")
        # dir 是 .../src/main
        # module_dir 是 .../src
        # 但 build.gradle 在 .../ 目录下
        module_dir=$(dirname "$(dirname "$dir")")
        build_gradle="$module_dir/build.gradle"
        
        if [ -f "$build_gradle" ]; then
            # 在 android { 后插入 namespace
            sed -i '' "/android {/a\\
    namespace '${pkg}'
" "$build_gradle"
        fi
    fi
done
