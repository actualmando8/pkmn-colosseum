    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    lwz r31, -20896(r13)
    bl fn_801653C4
    mr. r27, r3
    beq @L_8026628C
    bl fn_801656D8
    mr r29, r3
    li r3, 0x1
    li r4, 0x32
    li r5, 0xff
    bl fn_80165A20
    b @L_80266290
@L_8026628C:
    li r29, 0x0
@L_80266290:
    bl fn_801653BC
    mr. r28, r3
    beq @L_802662B0
    bl fn_801656D8
    mr r30, r3
    li r3, 0x32
    bl fn_801657D0
    b @L_802662B4
@L_802662B0:
    li r30, 0x0
@L_802662B4:
    li r3, 0x1
    bl fn_801DADC0
    mr r3, r31
    bl fn_80265F94
    bl fn_801DAC90
    cmplwi r27, 0x0
    beq @L_802662E0
    mr r3, r27
    clrlwi r5, r29, 24
    li r4, 0x32
    bl fn_80165A20
@L_802662E0:
    cmplwi r28, 0x0
    beq @L_802662F8
    mr r3, r28
    clrlwi r5, r30, 24
    li r4, 0x32
    bl fn_801659FC
@L_802662F8:
    bl fn_800FF660
    lis r4, 0x596
    li r3, 0x0
    addi r4, r4, 0x8
    bl fn_8011288C
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
