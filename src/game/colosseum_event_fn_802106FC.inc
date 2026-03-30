    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r3, r4
    li r4, 0x0
    stw r0, 0x14(r1)
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 16, 31
    cmpwi r0, 0x50
    beq @L_80210790
    bge @L_80210760
    cmpwi r0, 0x28
    beq @L_80210780
    bge @L_80210754
    cmpwi r0, 0x14
    beq @L_80210778
    b @L_802107A8
@L_80210754:
    cmpwi r0, 0x3c
    beq @L_80210788
    b @L_802107A8
@L_80210760:
    cmpwi r0, 0x78
    beq @L_802107A0
    bge @L_802107A8
    cmpwi r0, 0x64
    beq @L_80210798
    b @L_802107A8
@L_80210778:
    li r3, 0x0
    b @L_802107AC
@L_80210780:
    li r3, 0x1
    b @L_802107AC
@L_80210788:
    li r3, 0x2
    b @L_802107AC
@L_80210790:
    li r3, 0x3
    b @L_802107AC
@L_80210798:
    li r3, 0x4
    b @L_802107AC
@L_802107A0:
    li r3, 0x5
    b @L_802107AC
@L_802107A8:
    li r3, 0x0
@L_802107AC:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
