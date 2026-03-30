    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r3
    mr r28, r4
    mr r29, r5
    li r30, 0x0
    li r3, 0xe05
    bl fn_801903B0
    mr r3, r27
    bl fn_8020E0F8
    mr r0, r3
    li r3, 0x8ae
    mr r31, r0
    bl fn_801906A0
    cmplwi r3, 0x0
    beq @L_801CA650
    li r3, 0xe05
    bl fn_801903B0
    bl fn_8006A65C
    rlwinm r31, r3, 0, 16, 31
    li r3, 0x8ae
    bl fn_801906A0
    cmplwi r3, 0x0
    beq @L_801CA634
    bl fn_80112700
    b @L_801CA6E8
@L_801CA634:
    li r3, 0x0
    bl fn_801CBA90
    lis r4, 0x596
    li r3, 0x0
    addi r4, r4, 0x8
    bl fn_8011288C
    b @L_801CA6E8
@L_801CA650:
    rlwinm r0, r28, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801CA664
    cmpwi r29, 0x0
    beq @L_801CA66C
@L_801CA664:
    li r3, 0xe05
    bl fn_80190528
@L_801CA66C:
    mr r3, r27
    bl fn_8020DAD0
    mr r0, r3
    mr r3, r31
    mr r31, r0
    bl fn_8020DFD8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801CA6BC
    bl fn_801EF634
    mr r4, r3
    li r3, 0x0
    bl fn_801F1DBC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801CA6BC
    bl fn_800F0308
    bl fn_800F0308
    bl fn_800F0308
    li r30, 0x1
@L_801CA6BC:
    cmpwi r29, 0x0
    bne @L_801CA6E8
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801CA6E8
    rlwinm r0, r28, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801CA6E8
    lfs f1, -21912(r2)
    li r3, 0x2
    bl fn_801C41C8
@L_801CA6E8:
    li r3, 0xe05
    bl fn_801903B0
    rlwinm r3, r31, 0, 16, 31
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
