    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r27, 0x3c(r1)
    li r0, 0x1
    la r30, lbl_8047A360(r13)
    lis r3, lbl_802EF0A8@ha
    stw r0, 0x4(r30)
    addi r31, r3, lbl_802EF0A8@l
    li r27, 0x0
    li r29, 0x1
    @L_8002111C:
    cmpwi r27, 0x4
    beq @L_80021528
    bge @L_8002114C
    cmpwi r27, 0x1
    beq @L_8002116C
    bge @L_80021140
    cmpwi r27, 0x0
    bge @L_80021164
    b @L_8002156C
    @L_80021140:
    cmpwi r27, 0x3
    bge @L_800213D4
    b @L_80021364
    @L_8002114C:
    cmpwi r27, 0x64
    beq @L_8002154C
    bge @L_8002156C
    cmpwi r27, 0x6
    bge @L_8002156C
    b @L_80021244
    @L_80021164:
    li r27, 0x1
    b @L_8002156C
    @L_8002116C:
    li r0, 0x1
    li r3, 0x7f
    stw r0, 0x4(r30)
    li r4, 0x0
    bl fn_8010264C
    bl fn_801046B8
    mr r4, r3
    li r3, 0xaa
    la r5, lbl_8047A360(r13)
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    cmpwi r3, 0x0
    bge @L_800211B4
    li r0, -0x1
    stw r0, lbl_8047A360(r13)
    @L_800211B4:
    cmpwi r3, 0x0
    bne @L_800211C4
    li r27, 0x5
    b @L_8002156C
    @L_800211C4:
    lwz r0, lbl_8047A360(r13)
    cmpwi r0, 0x2
    beq @L_800211F4
    bge @L_800211E4
    cmpwi r0, 0x0
    beq @L_8002156C
    bge @L_80021214
    b @L_8002122C
    @L_800211E4:
    cmpwi r0, 0x4
    beq @L_8002121C
    bge @L_8002122C
    b @L_80021204
    @L_800211F4:
    li r3, 0x0
    bl fn_8005CEE8
    li r29, 0x0
    b @L_8002156C
    @L_80021204:
    li r3, 0x3
    bl fn_8005CEE8
    li r29, 0x0
    b @L_8002156C
    @L_80021214:
    li r27, 0x2
    b @L_8002156C
    @L_8002121C:
    li r0, 0x4
    li r27, 0x4
    stw r0, 0x4(r30)
    b @L_8002156C
    @L_8002122C:
    li r3, 0x1
    bl fn_800205B8
    li r3, 0x384
    bl fn_800FF58C
    li r27, 0x64
    b @L_8002156C
    @L_80021244:
    li r3, 0x0
    li r4, 0x4
    bl fn_80135168
    cmplwi r3, 0x0
    bne @L_80021260
    li r4, 0x3c3a
    b @L_80021280
    @L_80021260:
    li r3, 0x0
    bl fn_8006A718
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_8002127C
    li r4, 0x44c9
    b @L_80021280
    @L_8002127C:
    li r4, 0x3c35
    @L_80021280:
    li r3, 0x2
    li r5, 0x1
    li r6, 0x1
    bl fn_80106D3C
    li r0, 0x5
    li r3, 0x0
    stw r0, 0x4(r30)
    li r4, 0x3c
    li r5, 0xa6
    li r6, 0x1
    bl fn_8001E074
    extsb r27, r3
    li r3, 0x1
    bl fn_801069FC
    cmpwi r27, 0x0
    bne @L_8002135C
    li r3, 0xaa
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x7a
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x7f
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    lwz r3, lbl_8047A35C(r13)
    bl fn_801CB9D8
    lis r3, lbl_803A1FF8@ha
    addi r3, r3, lbl_803A1FF8@l
    bl fn_8010A420
    li r3, 0x1
    li r4, 0x3e8
    li r5, 0xff
    bl fn_80165A20
    li r3, 0x0
    li r4, 0x0
    bl fn_80029760
    addi r3, r1, 0x10
    bl fn_80029638
    addi r4, r1, 0x10
    li r3, 0x0
    bl fn_80128E38
    li r3, 0x1
    bl fn_800056E4
    lfs f1, lbl_8047B898(r2)
    bl fn_800056EC
    bl fn_80130054
    li r3, 0x3a0
    li r4, 0x0
    bl fn_80113828
    li r27, 0x64
    b @L_8002156C
    @L_8002135C:
    li r27, 0x1
    b @L_8002156C
    @L_80021364:
    li r3, 0x0
    bl fn_800056E4
    li r3, 0x0
    li r4, 0x4
    bl fn_80135168
    cmplwi r3, 0x0
    beq @L_80021388
    li r27, 0x3
    b @L_8002156C
    @L_80021388:
    bl fn_801D04E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_800213B0
    li r3, 0x2
    li r4, 0x44ea
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_800213C4
    @L_800213B0:
    li r3, 0x2
    li r4, 0x444b
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    @L_800213C4:
    li r3, 0x1
    bl fn_801069FC
    li r27, 0x1
    b @L_8002156C
    @L_800213D4:
    li r3, 0xaa
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x7f
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    lis r3, lbl_803A1FF8@ha
    lha r4, 0x4efe(r31)
    lha r5, 0x4f00(r31)
    addi r3, r3, lbl_803A1FF8@l
    bl fn_8010A5BC
    lis r3, lbl_803A1FF8@ha
    lis r4, 0xf7
    addi r3, r3, lbl_803A1FF8@l
    addi r4, r4, 0x400
    bl fn_8010A010
    lis r3, 0xf7
    addi r3, r3, 0x400
    bl fn_8018F6F4
    addi r5, r1, 0xc
    addi r6, r1, 0x8
    li r4, 0x1
    bl fn_8018F4C8
    lis r3, lbl_803A1FF8@ha
    lwz r4, 0xc(r1)
    addi r3, r3, lbl_803A1FF8@l
    bl fn_80109894
    bl fn_80005748
    mr r28, r3
    bl fn_801EF214
    mr r27, r3
    bl fn_801EF274
    mr r4, r28
    mr r28, r3
    li r3, 0x4c
    bl fn_80132A38
    mr r4, r28
    li r3, 0x2f
    bl fn_80132A38
    mr r4, r27
    li r3, 0x30
    bl fn_80132A38
    li r3, 0x16
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x1
    li r4, 0x3c22
    li r5, 0x0
    li r6, 0x1
    bl fn_80106D3C
    li r3, 0x0
    li r4, 0x3c
    li r5, 0xd6
    li r6, 0x1
    bl fn_8001E074
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_800214DC
    bl fn_80130054
    bl fn_80113FE8
    li r27, 0x64
    li r3, 0x1
    bl fn_800056E4
    b @L_80021500
    @L_800214DC:
    cmpwi r0, 0x1
    bne @L_800214F4
    li r27, 0x1
    li r3, 0x1
    bl fn_800056E4
    b @L_80021500
    @L_800214F4:
    li r27, 0x1
    li r3, 0x1
    bl fn_800056E4
    @L_80021500:
    li r3, 0x16
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x1
    bl fn_801069FC
    lis r3, lbl_803A1FF8@ha
    addi r3, r3, lbl_803A1FF8@l
    bl fn_8010A420
    b @L_8002156C
    @L_80021528:
    li r3, 0x7f
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    bl fn_80020C9C
    li r0, 0x1
    li r27, 0x1
    stw r0, 0x4(r30)
    b @L_8002156C
    @L_8002154C:
    lwz r0, lbl_8047A360(r13)
    cmpwi r0, 0x5
    bge @L_80021560
    cmpwi r0, 0x0
    bge @L_80021564
    @L_80021560:
    li r0, 0x0
    @L_80021564:
    stw r0, lbl_8047A358(r13)
    li r29, 0x0
    @L_8002156C:
    cmpwi r29, 0x0
    bne @L_8002111C
    li r3, 0xaa
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x7a
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x7f
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    lwz r3, lbl_8047A35C(r13)
    bl fn_801CB9D8
    lwz r3, lbl_8047A360(r13)
    lmw r27, 0x3c(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
