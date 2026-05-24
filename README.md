# metaseq-physics-plugin

Blender 風の物理シミュレーション（布・剛体・衝突体・ソフトボディ）を
[Metasequoia 4](https://www.metaseq.net/) のプラグインとして実装するプロジェクトです。

> **ステータス: Phase 1 (MVP) — スキャフォールド構築中**
> 現時点では、Station プラグインの土台・Bullet ラッパ・布シミュレーションの
> 最小ループまでを実装します。実機での挙動検証は Windows + Metasequoia で
> ご自身でご確認ください。

## 機能（Phase 1 計画）

- Metasequoia の **Station プラグイン** として常駐し、ドッキングウィンドウを提供
- シーン内オブジェクトに「布 / 剛体 / 衝突体」のロールを割り当て
- シミュレーション 再生 / 停止 / リセット（固定タイムステップ 60 Hz）
- 布シミュレーション（ピン頂点はマテリアルまたは選択頂点で指定）

ロードマップ（Phase 2 以降）は本 README の末尾を参照。

## ビルド

### 必要環境

- Windows 10/11 x64
- Visual Studio 2022 (MSVC, C++20)
- CMake 3.21 以上
- Git（サブモジュール用）
- Metasequoia 4 SDK（ご自身で取得・配置）

### 手順

1. このリポジトリをクローン後、Bullet Physics をサブモジュールとして取得します。

   ```sh
   git submodule update --init --recursive
   ```

2. Metasequoia SDK を <https://www.metaseq.net/jp/download/sdk/> から取得し、
   `sdk/mqsdk/` 配下に **そのままの構成で** 配置します（再配布禁止のため
   本リポジトリには同梱していません）。最終的に以下のような配置を想定します。

   ```text
   sdk/
     mqsdk/
       MQPlugin.h
       MQWidget.h
       MQBasePlugin.h
       MQ3DLib.h
       mqsdk.cpp
       ...
   ```

3. CMake でビルドします（x64 / Release）。

   ```sh
   cmake -S . -B build -A x64
   cmake --build build --config Release
   ```

4. 生成された `build/Release/MetaseqPhysics.dll` を Metasequoia の
   `Plugins/Station/` フォルダにコピーすると、メニューから本プラグインの
   ウィンドウを開けます。

### SDK が無い環境でのコンパイル確認（CI 用）

SDK 未配置時は、`tests/mock_sdk/` 配下の **API シグネチャだけを真似たモックヘッダ**
を用いて構文チェックビルドが実行できます。これはあくまでコンパイルが通ることを
確認するためのもので、生成された DLL は Metasequoia では動作しません。

```sh
cmake -S . -B build -DMQ_USE_MOCK_SDK=ON
cmake --build build
```

GitHub Actions の `windows-ci` ワークフローはこのモードで実行しています。

## ディレクトリ構成

```text
metaseq-physics-plugin/
├── CMakeLists.txt
├── README.md
├── LICENSE
├── cmake/
│   └── FindMQSDK.cmake
├── extern/
│   └── bullet3/                (git submodule)
├── sdk/
│   └── mqsdk/                  (利用者が手動配置; .gitignore 済)
├── src/
│   ├── PluginMain.cpp
│   ├── PhysicsStation.{h,cpp}
│   ├── PhysicsWorld.{h,cpp}
│   ├── ClothBody.{h,cpp}
│   ├── SoftBody.{h,cpp}
│   ├── RigidBody.{h,cpp}
│   ├── Collider.{h,cpp}
│   ├── SimulationCache.{h,cpp}
│   ├── PropertyStore.{h,cpp}
│   └── ui/
│       ├── PhysicsPanel.{h,cpp}
│       └── ResourceStrings.{h,cpp}
├── res/
│   └── plugin.rc
└── tests/
    ├── mock_sdk/               (CI 用モック SDK)
    └── unit/
```

## ライセンス

- 本プラグインのコード: MIT License（`LICENSE` 参照）
- Metasequoia SDK: tetraface Inc. の利用規約に従う（再配布不可）
- Bullet Physics: zlib License（submodule に同梱）

## ロードマップ

| Phase | 内容 |
| --- | --- |
| 1 (現在) | プラグイン雛形 + 布シミュレーション最小動作 |
| 2 | ソフトボディ、自己衝突、衝突マージン、風・重力 UI |
| 3 | フレームベイク（モーフ / オブジェクト履歴への書き出し） |
| 4 | プリセット保存、シーンへの物理設定の永続化（サイドカー JSON） |
