    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r30, r3
    mr r27, r4
    bl fn_801040A0
    lwz r0, 0x4(r30)
    mr r31, r3
    cmpwi r0, 0x50
    bne @L_80105904
    lha r29, 0xc(r31)
    lha r28, 0xe(r31)
    b @L_80105914
@L_80105904:
    lha r3, 0x54(r27)
    lha r0, 0x56(r27)
    mr r29, r3
    mr r28, r0
@L_80105914:
    lha r3, 0x56(r27)
    lha r0, 0x52(r27)
    subi r3, r3, 0x4
    lha r6, 0x84(r30)
    subf r3, r28, r3
    lha r5, 0x50(r27)
    extsh r4, r3
    lha r3, 0x86(r30)
    add r0, r0, r4
    add r4, r6, r5
    add r0, r3, r0
    extsh r3, r4
    extsh r4, r0
    bl fn_800FE6D0
    bl fn_800FE4D4
    lwz r0, 0x4(r30)
    cmpwi r0, 0x52
    bge @L_80105974
    cmpwi r0, 0x40
    beq @L_80105980
    blt @L_801059BC
    cmpwi r0, 0x50
    bge @L_80105980
    b @L_801059BC
@L_80105974:
    cmpwi r0, 0x10c
    beq @L_801059A0
    b @L_801059BC
@L_80105980:
    lbz r7, 0x8b(r30)
    extsh r5, r29
    extsh r6, r28
    li r3, 0x0
    li r4, 0x2
    li r8, 0x1
    bl fn_8001EC08
    b @L_801059BC
@L_801059A0:
    lbz r7, 0x8b(r30)
    extsh r5, r29
    extsh r6, r28
    li r3, 0x0
    li r4, 0x2
    li r8, 0x0
    bl fn_8001EC08
@L_801059BC:
    lbz r0, 0x1(r30)
    extsb r0, r0
    cmpwi r0, 0x3
    beq @L_80105A24
    bge @L_801059E0
    cmpwi r0, 0x2
    bge @L_801059EC
    cmpwi r0, 0x0
    b @L_80105A24
@L_801059E0:
    cmpwi r0, 0x5
    beq @L_80105A24
    b @L_80105A24
@L_801059EC:
    lwz r0, 0x4(r30)
    cmpwi r0, 0x51
    beq @L_80105A24
    lbz r0, 0x0(r30)
    lwz r3, 0x0(r31)
    extsb r0, r0
    lwz r4, 0x8(r31)
    rlwinm r0, r0, 0, 29, 29
    cntlzw r0, r0
    rlwinm r0, r0, 27, 5, 31
    rlwinm r5, r0, 0, 24, 31
    bl fn_800FBE7C
    li r0, 0x0
    stw r0, 0x8(r31)
@L_80105A24:
    li r3, 0x0
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
