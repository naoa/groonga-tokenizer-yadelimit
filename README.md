# Yet another delimited tokenizer plugin for Groonga

## Tokenizer

### ``TokenYaDelimitCaret``

It tokenize by splliting with a caret ``^``.

```
> tokenize TokenYaDelimitCaret "a^b"
[[0,1408307554.89682,0.00087738037109375],
[
{"value":"a","position":0},
{"value":"b","position":1}
]]
```

### ``TokenYaDelimitColon``

It tokenize by splliting with a colon ``:``.

```
> tokenize TokenYaDelimitColon "a:b"
[[0,1408307603.1058,0.000877618789672852],
[
{"value":"a","position":0},
{"value":"b","position":1}
]]
```

### ``TokenYaDelimitSemicolon``

It tokenize by splliting with a semicolon ``;``.

```
> tokenize TokenYaDelimitSemicolon "a;b"
[[0,1408307632.20784,0.000780344009399414],
[
{"value":"a","position":0},
{"value":"b","position":1}
]]
```

### ``TokenYaDelimitPunct``

It tokenize by splliting with punctuations ``!"#$%&'()*+,-./:;<=>?@[\]^_`{|}``.

```
> tokenize TokenYaDelimitPunct "a@b"
[[0,1408307668.55782,0.00076603889465332],
[
{"value":"a","position":0},
{"value":"b","position":1}
]]
```

### ``TokenYaDelimitPunctWithBlank``

It tokenize by splliting with punctuations and blank ``!"#$%&'()*+,-./:;<=>?@[\]^_`{|}  ``.

```
> tokenize TokenYaDelimitPunctWithBlank "This is a pen. Do you understand?"
[[0,1408307738.93184,0.00175285339355469],
[
{"value":"This","position":0},
{"value":"is","position":1},
{"value":"a","position":2},
{"value":"pen","position":3},
{"value":"Do","position":4},
{"value":"you","position":5},
{"value":"understand","position":6}
]]
```

## Install

Install ``groonga-tokenizer-yadelimit`` package:

### CentOS

* CentOS6

```
% sudo yum localinstall -y http://packages.createfield.com/centos/6/groonga-tokenizer-yadelimit-1.0.0-1.el6.x86_64.rpm
```

* CentOS7

```
% sudo yum localinstall -y http://packages.createfield.com/centos/7/groonga-tokenizer-yadelimit-1.0.0-1.el7.centos.x86_64.rpm
```

### Fedora

* Fedora 20

```
% sudo yum localinstall -y http://packages.createfield.com/fedora/20/groonga-tokenizer-yadelimit-1.0.0-1.fc20.x86_64.rpm
```

* Fedora 21

```
% sudo yum localinstall -y http://packages.createfield.com/fedora/21/groonga-tokenizer-yadelimit-1.0.0-1.fc21.x86_64.rpm
```

### Debian GNU/LINUX

* wheezy

```
% wget http://packages.createfield.com/debian/wheezy/groonga-tokenizer-yadelimit_1.0.0-1_amd64.deb
% sudo dpkg -i groonga-tokenizer-yadelimit_1.0.0-1_amd64.deb
```

* jessie

```
% wget http://packages.createfield.com/debian/jessie/groonga-tokenizer-yadelimit_1.0.0-1_amd64.deb
% sudo dpkg -i groonga-tokenizer-yadelimit_1.0.0-1_amd64.deb
```


### Ubuntu

* precise

```
% wget http://packages.createfield.com/ubuntu/precise/groonga-tokenizer-yadelimit_1.0.0-1_amd64.deb
% sudo dpkg -i groonga-tokenizer-yadelimit_1.0.0-1_amd64.deb
```

* trusty

```
% wget http://packages.createfield.com/ubuntu/trusty/groonga-tokenizer-yadelimit_1.0.0-1_amd64.deb
% sudo dpkg -i groonga-tokenizer-yadelimit_1.0.0-1_amd64.deb
```

### Source install

Build this tokenizer.

    % sh autogen.sh
    % ./configure
    % make
    % sudo make install

## Dependencies

* Groonga >= 4.0.5

Install ``groonga-devel`` in CentOS/Fedora. Install ``libgroonga-dev`` in Debian/Ubutu.

See http://groonga.org/docs/install.html

## Usage

Firstly, register `tokenizers/yadelimit`

Groonga:

    % groonga db
    > register tokenizers/yadelimit
    > table_create Diaries TABLE_HASH_KEY INT32
    > column_create Diaries body COLUMN_SCALAR TEXT
    > table_create Terms TABLE_PAT_KEY ShortText --default_tokenizer TokenYaDelimitPunctWithBlank
    > column_create Terms diaries_body COLUMN_INDEX|WITH_POSITION Diaries body

Mroonga:

    mysql> use db;
    mysql> CREATE TABLE `temp` (id INT NOT NULL) ENGINE=mroonga DEFAULT CHARSET=utf8;
    mysql> DROP TABLE `temp`;
    mysql> select mroonga_command("register tokenizers/yadelimit");
    mysql> CREATE TABLE `Diaries` (
        -> id INT NOT NULL,
        -> body TEXT NOT NULL,
        -> PRIMARY KEY (id) USING HASH,
        -> FULLTEXT INDEX (body) COMMENT 'parser "TokenYaDelimitPunctWithBlank"'
        -> ) ENGINE=mroonga DEFAULT CHARSET=utf8;

Rroonga:

    irb --simple-prompt -rubygems -rgroonga
    >> Groonga::Context.default_options = {:encoding => :utf8}   
    >> Groonga::Database.create(:path => "/tmp/db")
    >> Groonga::Plugin.register(:path => "tokenizers/yadelimit.so")
    >> Groonga::Schema.create_table("Diaries",
    ?>                              :type => :hash,
    ?>                              :key_type => :integer32) do |table|
    ?>   table.text("body")
    >> end
    >> Groonga::Schema.create_table("Terms",
    ?>                              :type => :patricia_trie,
    ?>                              :normalizer => :NormalizerAuto,
    ?>                              :default_tokenizer => "TokenYaDelimitPunctWithBlank") do |table|
    ?>   table.index("Diaries.body")
    >> end
    
## Author

* Naoya Murakami <naoya@createfield.com>

## License

LGPL 2.1. See COPYING for details.

This program includes the Groonga delimited tokenizer.  
https://github.com/groonga/groonga/blob/master/lib/token.c

This program is the same license as Groonga.

The Groonga delimited tokenizer is povided by ``Copyright(C) 2009-2012 Brazil``
