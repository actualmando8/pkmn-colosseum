    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r29, 0x24(r1)
    mr. r31, r3
    beq @L_802073AC
    li r4, 0x0
    li r5, 0xd5
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0xd7
    li r6, 0x0
    bl fn_8012640C
    mr. r29, r3
    beq @L_80206DB4
    li r4, 0x0
    li r5, 0xcb
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r29
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    bl fn_80124A60
    mr r3, r29
    li r4, 0x0
    li r5, 0xcd
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x1
    bl fn_8011B950
    mr r3, r29
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    li r7, -0x1
    bl fn_801254B4
    mr r3, r29
    li r4, 0x0
    li r5, 0xcf
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r29
    li r4, 0x0
    li r5, 0xd0
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r29
    li r4, 0x0
    li r5, 0xd1
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r29
    li r4, 0x0
    li r5, 0xd2
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
@L_80206DB4:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd8
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x34
    bl fn_8011B950
    lis r3, -32728
    addi r30, r1, 0x8
    addi r6, r3, -25504
    li r29, 0x0
    lwz r5, 0x0(r6)
    lwz r4, 0x4(r6)
    lwz r3, 0x8(r6)
    lhz r0, 0xc(r6)
    stw r5, 0x8(r1)
    stw r4, 0xc(r1)
    stw r3, 0x10(r1)
    sth r0, 0x14(r1)
    b @L_80206E24
@L_80206E04:
    clrlslwi r0, r29, 24, 1
    mr r3, r31
    lhzx r5, r30, r0
    li r4, 0x0
    li r6, 0x0
    li r7, 0x6
    bl fn_801254B4
    addi r29, r29, 0x1
@L_80206E24:
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0x7
    blt @L_80206E04
    mr r3, r31
    li r4, 0x0
    bl fn_801FD830
    mr r3, r31
    li r4, 0x0
    li r5, 0xed
    li r6, 0x0
    li r7, 0x2
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    li r29, 0x0
    b @L_80206E90
@L_80206E74:
    mr r3, r31
    rlwinm r6, r29, 0, 24, 31
    li r4, 0x0
    li r5, 0xfd
    li r7, -0x1
    bl fn_801254B4
    addi r29, r29, 0x1
@L_80206E90:
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0xc
    blt @L_80206E74
    mr r3, r31
    li r4, 0x0
    li r5, 0xfe
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    beq @L_80206EEC
    bl fn_801F1460
    mr r3, r31
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    bl fn_80209D90
    mr r3, r31
    li r4, 0x0
    li r5, 0xe5
    li r6, 0x0
    bl fn_8012640C
    bl fn_8020A478
@L_80206EEC:
    mr r3, r31
    li r4, 0x0
    li r5, 0xf8
    li r6, 0x0
    bl fn_8012640C
    bl fn_80209D90
    li r29, 0x0
    b @L_80206F28
@L_80206F0C:
    mr r3, r31
    rlwinm r6, r29, 0, 24, 31
    li r4, 0x0
    li r5, 0xff
    li r7, 0x9
    bl fn_801254B4
    addi r29, r29, 0x1
@L_80206F28:
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0x2
    blt @L_80206F0C
    mr r3, r31
    li r4, 0x0
    li r5, 0x100
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x101
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    beq @L_80206F70
    li r0, 0x0
    stw r0, 0x0(r3)
@L_80206F70:
    mr r3, r31
    li r4, 0x0
    li r5, 0xef
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0xf0
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0xf1
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0xf2
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0xf3
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0xf4
    li r6, 0x0
    li r7, 0x9
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0xf5
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0xf6
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0xf7
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0xf9
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0xfc
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0xfb
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x102
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x103
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x104
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x105
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x106
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x107
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x108
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x109
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x10a
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x10b
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x10c
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x10d
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x10e
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x10f
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x110
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x111
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x112
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x113
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x114
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x115
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x116
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x117
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x118
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x119
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x11a
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x11b
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x11c
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x11d
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x11e
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x11f
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x120
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x121
    li r6, 0x0
    li r7, -0x1
    bl fn_801254B4
    mr r3, r31
    li r4, 0x0
    li r5, 0x122
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x4
    bl fn_8020E6D4
@L_802073AC:
    lmw r29, 0x24(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
