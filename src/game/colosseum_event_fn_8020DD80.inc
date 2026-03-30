    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r30, r3
    bl fn_8020E0F8
    mr r31, r3
    bl fn_8020E008
    cmplwi r3, 0x0
    beq @L_8020DDAC
    b @L_8020DE3C
@L_8020DDAC:
    mr r3, r30
    bl fn_8020E0F8
    bl fn_8020E0B0
    mr r0, r3
    li r3, 0x0
    mr r4, r0
    li r5, 0x6
    li r6, 0x0
    bl fn_801F54A4
    cmplwi r3, 0x0
    beq @L_8020DDDC
    b @L_8020DE3C
@L_8020DDDC:
    li r30, 0x0
    b @L_8020DE2C
@L_8020DDE4:
    mr r3, r31
    clrlwi r4, r30, 24
    bl fn_8020E068
    clrlwi. r0, r3, 16
    beq @L_8020DE28
    mr r4, r3
    li r3, 0x0
    li r5, 0x4
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi. r3, r3, 16
    beq @L_8020DE28
    bl fn_801FBD58
    bl fn_801FBD10
    cmplwi r3, 0x0
    beq @L_8020DE28
    b @L_8020DE3C
@L_8020DE28:
    addi r30, r30, 0x1
@L_8020DE2C:
    clrlwi r0, r30, 16
    cmplwi r0, 0x4
    blt @L_8020DDE4
    li r3, 0x1
@L_8020DE3C:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
