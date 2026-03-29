    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r28, r3
    mr r29, r4
    mr r30, r5
    lbz r0, 0x44(r28)
    cmplwi r0, 0x0
    bne @L_800117EC
    li r31, 0x4c
    b @L_800117F0
    @L_800117EC:
    li r31, 0xf7
    @L_800117F0:
    mr r3, r31
    li r4, 0x1e
    bl fn_801080CC
    mr r3, r31
    mr r5, r29
    mr r7, r30
    mr r9, r28
    li r4, 0x0
    li r6, 0x0
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    mr r27, r3
    cmpwi r27, 0x0
    blt @L_80011984
    mr r3, r28
    clrlwi r4, r27, 16
    bl fn_80010128
    mr r26, r3
    cmplwi r26, 0x0
    beq @L_80011984
    li r3, 0x4c
    bl fn_80102620
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80011868
    li r3, 0x4c
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    @L_80011868:
    li r3, 0xf9
    bl fn_80102620
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8001188C
    li r3, 0xf9
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    @L_8001188C:
    li r3, 0xfa
    bl fn_80102620
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_800118B0
    li r3, 0xfa
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    @L_800118B0:
    li r3, 0xf7
    bl fn_80102620
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_800118D4
    li r3, 0xf7
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    @L_800118D4:
    mr r3, r26
    li r4, 0x1
    li r5, 0x1
    bl fn_80106394
    b @L_80011954
    @L_800118E8:
    li r3, 0x0
    bl fn_801F18DC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80011940
    li r3, 0x0
    bl fn_801F1700
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80011928
    bl fn_80265924
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80011928
    li r0, 0x1
    b @L_80011944
    @L_80011928:
    bl fn_801EF634
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_80011940
    li r0, 0x1
    b @L_80011944
    @L_80011940:
    li r0, 0x0
    @L_80011944:
    clrlwi r0, r0, 24
    cmplwi r0, 0x0
    bne @L_80011978
    bl fn_800F0308
    @L_80011954:
    li r3, 0x1
    bl fn_800F7AF0
    mr r27, r3
    li r3, 0x1
    bl fn_800F7BC4
    and r0, r3, r27
    rlwinm r0, r0, 0, 22, 23
    cmplwi r0, 0x0
    beq @L_800118E8
    @L_80011978:
    li r3, 0x1
    bl fn_80106080
    b @L_800117F0
    @L_80011984:
    mr r3, r31
    li r4, 0x20
    bl fn_801080CC
    mr r3, r27
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
