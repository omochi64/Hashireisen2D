Hashireisen2D
==========

※English follows below.

## Overview
2009年に公開した、「はしれーせん!!」( http://www.ricecake.st/game/hashireisen/index.html ) のソースコードです。

はしれーせん!! 3Dの方ではなく、サイトで無料で公開しているゲームのコードです。

（このリポジトリを作成した2015年の丁度6年ぐらい前に公開という、随分昔のものです）。

## Description
以下の3つのプロジェクトがソリューション内に入っています。
* Hashireisen: ゲームのメインプロジェクトです
* Config: キーコンフィグ、起動時オプションを設定する外部ツールです
* MapUtility: "はしれーせん!! 3D" に使用したマップツールです ("はしれーせん!!" には用いていませんが何故かこのソリューション内に入っていたので一緒に入れています)

ライブラリとして、Selene1.0系 (現在公開されていません) 、TinyXMLを利用しています。

ゲームの中身についてはWebサイトを見てください  http://www.ricecake.st/game/hashireisen/index.html

## Build
ビルドには以下が必要です。
* Visual Studio 2013
* Microsoft DirectX SDK http://www.microsoft.com/en-us/download/details.aspx?id=6812

Hashireisen.sln を Visual Studio で開き、ソリューションをビルドすることで実行可能ファイルが bin ディレクトリ以下に出力されます。

bin 以下に生成された exe を直接実行することで起動できます。

## Usage
ゲームの遊び方については、サイトで公開しているマニュアルを見てください。

http://www.ricecake.st/game/hashireisen/manual.html

## Author
お餅 (@omochi64)

ゲームについて問い合わせる際は、お餅へ直接問い合わせるか、info@ricecake.st へご連絡ください。

## License
本作品は、上海アリス幻樂団の「東方Project」の二次創作です。

また、この作品の公開にあたり、一部以下のWebサイトのフリー素材を利用させていただきました。

* ドット絵
    * 点睛集積　http://do-t.cool.ne.jp/dots/
    * はちみつくまさん はちくま東方素材蒐　http://www.hachikuma.net/
    * もず倉　http://members.jcom.home.ne.jp/mozunatto/
* 背景
    * 写真素材 足成　http://www.9031.com/
* フォント
    * 9031.com　http://www.9031.com/
* BGM
    * SAM Free Music　http://sam-free.com/
    * TAM Music Factory　http://www.tam-music.com/
    * 音の葉っぱ　http://www.geocities.jp/spacheeg/
* 効果音
    * ザ・マッチメイカァズ　http://osabisi.sakura.ne.jp/m2/
    * ユウラボ8bitサウンド工房　http://www.skipmore.com/sound/


<pre>
「はしれーせん!!」はOgg Vorbis SDKを使用しています。
------------------------------------------------------
THE OggVorbis SOURCE CODE IS (C) COPYRIGHT 1994-2007 by the Xiph.org Foundation, http://www.xiph.org/
------------------------------------------------------
</pre>

<pre>
「はしれーせん!! 3D 体験版3」はTinyXMLを使用しています
------------------------------------------------------
www.sourceforge.net/projects/tinyxml
Original code (2.0 and earlier )copyright (c) 2000-2006 Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source distribution.
------------------------------------------------------
</pre>

==========
English version.

## Overview

The project "Hashireisen 2D" ( http://www.ricecake.st/game/hashireisen/index.html ) source code. 

(The game is opened to the public in 2009, six years ago. Six year ago that I made this repository.)

## Description
This solution includes three C++ projects.
* Hashireisen: Main project of the game.
* Config: Key-config and setting options program project.
* MapUtility: Map utility tool which is used for "Hashireisen!! 3D". (This is not used for "Hashireisen 2D" but included in the solusion. Therefore I also commit this project.)

These projects use libraries, Selene ver.1.0 (this library is not opened already) and TinyXML.

See the web site about the game: http://www.ricecake.st/game/hashireisen/index.html

## Build
Following are required to build.
* Visual Studio 2013
* Microsoft DirectX SDK http://www.microsoft.com/en-us/download/details.aspx?id=6812

Open Hashireisen.sln by Visual Studio 2013, and build the solution, then .exe will be output to bin directory.

## Usage
See the manual on the web site about how to play the game.

http://www.ricecake.st/game/hashireisen/manual.html

## Author
omochi (@omochi64)

Contant @omochi64 or send an e-mail to info@ricecake.st if you have questions about the game.

## License
This game is a derivative work from Toho Project by Shanghai Alice.

Free materials are used in this game, which are distributed on the web sites below.

* Pixel art
    * 点睛集積　http://do-t.cool.ne.jp/dots/
    * はちみつくまさん はちくま東方素材蒐　http://www.hachikuma.net/
    * もず倉　http://members.jcom.home.ne.jp/mozunatto/
* Background graphics
    * 写真素材 足成　http://www.9031.com/
* Font
    * 9031.com　http://www.9031.com/
* BGM
    * SAM Free Music　http://sam-free.com/
    * TAM Music Factory　http://www.tam-music.com/
    * 音の葉っぱ　http://www.geocities.jp/spacheeg/
* SE
    * ザ・マッチメイカァズ　http://osabisi.sakura.ne.jp/m2/
    * ユウラボ8bitサウンド工房　http://www.skipmore.com/sound/


<pre>
「Hashireisen !!」 uses Ogg Vorbis SDK.
------------------------------------------------------
THE OggVorbis SOURCE CODE IS (C) COPYRIGHT 1994-2007 by the Xiph.org Foundation, http://www.xiph.org/
------------------------------------------------------
</pre>

<pre>
「Hashireisen !!」uses TinyXML.
------------------------------------------------------
www.sourceforge.net/projects/tinyxml
Original code (2.0 and earlier )copyright (c) 2000-2006 Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
------------------------------------------------------
</pre>
