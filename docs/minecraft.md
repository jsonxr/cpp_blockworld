# Minecraft Implementation

- Particle effects 1 chunk radius (3x3)
- Entities like sheep 160 block radius from player
- 40 block radius for items

# Resources

- [Minecraft source attribution](https://www.minecraft.net/en-us/attribution) - list of libraries used by Minecraft
  bedrock.
- [Definitive Guide to Creating a Window | How to Code Minecraft Ep. 1](https://www.youtube.com/watch?v=HhH_9DnMBgw)
- [Faithful 1.18 Resource Pack](https://faithful.team/faithful-1-18/)

# Useful Libraries used

- [microprofile](https://github.com/zeux/microprofile) - CPU/GPU profiling visualization tool

# Modding

- [Minecraft Forge Documentation](https://mcforge.readthedocs.io/en/1.18.x/) - Documenation on the Minecraft Forge apis
- [modding tutorial](https://www.youtube.com/watch?v=benUZaQU-6Y&list=PLlbkaeFHn13HQlW5Pb7Gf-xLJoAlfVbNU)
- [tint index](https://forums.minecraftforge.net/topic/29532-18-solved-tintindex-anyone-gotten-it-to-do-anything-useful/)
    - How to blend color for grass
- [Minecraft JSON](https://github.com/thecodewarrior-archive/MineCraft-Draw) - 7 year old site but has some missing json
  things like bed.json
- [Microsoft Minecraft Bedrock JSON](https://docs.microsoft.com/en-us/minecraft/creator/reference/)
- [Minecraft shaders](https://minecraft.fandom.com/wiki/Shaders)
- [Minecraft Modding - Block Models](http://greyminecraftcoder.blogspot.com/2014/12/block-models-18.html) Information on
  blocks
- [Minecraft Modding - texturing quads](http://greyminecraftcoder.blogspot.com/2014/12/block-models-texturing-quads-faces.html)
- [Minecraft Modding - Lighting](http://greyminecraftcoder.blogspot.com/2014/12/lighting-18.html)
- [Minecraft Block Models](https://minecraft.fandom.com/wiki/Model#Block_models)
- [Minecraft Block file format](https://minecraft.fandom.com/wiki/Structure_Block_file_format)
- [Minecraft Development Resources](https://minecraft.fandom.com/wiki/Development_resources)

# Example

```javascript
// acacia_log.json
{
    "parent"
:
    "minecraft:block/cube_column",
        "textures"
:
    {
        "end"
    :
        "minecraft:block/acacia_log_top",
            "side"
    :
        "minecraft:block/acacia_log"
    }
}

// cube_column.json
{
    "parent"
:
    "block/cube",
        "textures"
:
    {
        "particle"
    :
        "#side",
            "down"
    :
        "#end",
            "up"
    :
        "#end",
            "north"
    :
        "#side",
            "east"
    :
        "#side",
            "south"
    :
        "#side",
            "west"
    :
        "#side"
    }
}

// cube.json
{
    "parent"
:
    "block/block",
        "elements"
:
    [
        {
            "from": [0, 0, 0],
            "to": [16, 16, 16],
            "faces": {
                "down": {"texture": "#down", "cullface": "down"},
                "up": {"texture": "#up", "cullface": "up"},
                "north": {"texture": "#north", "cullface": "north"},
                "south": {"texture": "#south", "cullface": "south"},
                "west": {"texture": "#west", "cullface": "west"},
                "east": {"texture": "#east", "cullface": "east"}
            }
        }
    ]
}

// block.json
{
    "gui_light"
:
    "side",
        "display"
:
    {
        "gui"
    :
        {
            "rotation"
        :
            [30, 225, 0],
                "translation"
        :
            [0, 0, 0],
                "scale"
        :
            [0.625, 0.625, 0.625]
        }
    ,
        "ground"
    :
        {
            "rotation"
        :
            [0, 0, 0],
                "translation"
        :
            [0, 3, 0],
                "scale"
        :
            [0.25, 0.25, 0.25]
        }
    ,
        "fixed"
    :
        {
            "rotation"
        :
            [0, 0, 0],
                "translation"
        :
            [0, 0, 0],
                "scale"
        :
            [0.5, 0.5, 0.5]
        }
    ,
        "thirdperson_righthand"
    :
        {
            "rotation"
        :
            [75, 45, 0],
                "translation"
        :
            [0, 2.5, 0],
                "scale"
        :
            [0.375, 0.375, 0.375]
        }
    ,
        "firstperson_righthand"
    :
        {
            "rotation"
        :
            [0, 45, 0],
                "translation"
        :
            [0, 0, 0],
                "scale"
        :
            [0.4, 0.4, 0.4]
        }
    ,
        "firstperson_lefthand"
    :
        {
            "rotation"
        :
            [0, 225, 0],
                "translation"
        :
            [0, 0, 0],
                "scale"
        :
            [0.4, 0.4, 0.4]
        }
    }
}

```
