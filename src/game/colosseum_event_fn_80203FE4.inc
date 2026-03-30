    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    li r5, 0xd6
    stw r0, 0x14(r1)
    li r6, 0x0
    stmw r30, 0x8(r1)
    bl fn_8012640C
    mr. r30, r3
    bne @L_80204014
    li r31, 0x0
    b @L_80204028
@L_80204014:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r31, r3
@L_80204028:
    cmplwi r31, 0x0
    bne @L_80204038
    li r3, 0x0
    b @L_802040D4
@L_80204038:
    li r3, 0x3d
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x7c
    beq @L_80204060
    li r3, 0x3d
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xc8
    bne @L_80204090
@L_80204060:
    cmplwi r30, 0x0
    bne @L_80204070
    li r3, 0x0
    b @L_80204084
@L_80204070:
    mr r3, r30
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80204084:
    li r4, 0x3d
    bl fn_80121ADC
    b @L_802040B8
@L_80204090:
    li r3, 0x3d
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xcd
    beq @L_802040AC
    li r3, 0x0
    b @L_802040B8
@L_802040AC:
    mr r3, r30
    li r4, 0x3d
    bl fn_8011B67C
@L_802040B8:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802040CC
    li r3, 0x0
    b @L_802040D4
@L_802040CC:
    mr r3, r31
    bl fn_80123090
@L_802040D4:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
