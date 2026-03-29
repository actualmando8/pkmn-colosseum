    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    li r0, 0x0
    lwz r3, lbl_80478DDC(r13)
    stw r0, 0x8(r1)
    stw r0, lbl_8047A368(r13)
    lwz r3, 0x8(r3)
    bl fn_8005D934
    li r28, 0x0
    stw r3, lbl_8047A390(r13)
    slwi r31, r28, 4
    li r29, 0x0
    b @L_800257F8
    @L_80025778:
    lwz r0, lbl_80478DDC(r13)
    lwz r3, lbl_8047A3AC(r13)
    add r4, r0, r31
    lwz r0, 0x4(r4)
    cmplw r3, r0
    bne @L_800257A4
    stw r28, lbl_8047A368(r13)
    stw r28, 0x8(r1)
    lwz r3, 0x8(r4)
    bl fn_8005D934
    stw r3, lbl_8047A390(r13)
    @L_800257A4:
    lwz r3, lbl_80478DDC(r13)
    li r30, 0x1
    lwzx r3, r3, r31
    cmplwi r3, 0x0
    beq @L_800257CC
    bl fn_801902E0
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_800257CC
    li r30, 0x0
    @L_800257CC:
    lwz r3, lbl_80478DDC(r13)
    addi r0, r31, 0x8
    mr r4, r30
    lwzx r3, r3, r0
    bl fn_8005D8F8
    clrlwi r0, r30, 24
    cmplwi r0, 0x0
    beq @L_800257F0
    addi r28, r28, 0x1
    @L_800257F0:
    addi r31, r31, 0x10
    addi r29, r29, 0x1
    @L_800257F8:
    lwz r3, lbl_80478DD8(r13)
    lwz r0, 0x0(r3)
    cmplw r29, r0
    blt @L_80025778
    bl fn_801046B8
    mr r4, r3
    addi r5, r1, 0x8
    li r3, 0xbd
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    cmpwi r3, 0x0
    bge @L_80025840
    bl fn_8011394C
    mr r30, r3
    b @L_800258C0
    @L_80025840:
    lwz r4, lbl_80478DD8(r13)
    li r7, 0x0
    lwz r6, lbl_80478DDC(r13)
    lwz r5, 0x0(r4)
    mr r4, r6
    mtctr r5
    cmplwi r5, 0x0
    ble @L_80025878
    @L_80025860:
    lwz r0, 0x8(r4)
    cmplw r3, r0
    beq @L_80025878
    addi r4, r4, 0x10
    addi r7, r7, 0x1
    bdnz @L_80025860
    @L_80025878:
    cmplw r7, r5
    blt @L_80025884
    li r7, 0x0
    @L_80025884:
    slwi r31, r7, 4
    add r3, r6, r31
    lwz r0, 0x4(r3)
    cmplwi r0, 0x66
    bne @L_800258B4
    li r3, 0x45d
    bl fn_801902E0
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_800258B4
    li r30, 0x7b
    b @L_800258C0
    @L_800258B4:
    lwz r0, lbl_80478DDC(r13)
    add r3, r0, r31
    lwz r30, 0x4(r3)
    @L_800258C0:
    bl fn_8011394C
    cmplw r30, r3
    beq @L_8002590C
    cmplwi r30, 0x7a
    bne @L_800258E8
    bl fn_8011394C
    cmplwi r3, 0xf
    bne @L_800258E8
    li r0, 0x0
    b @L_80025910
    @L_800258E8:
    cmplwi r30, 0xf
    bne @L_80025904
    bl fn_8011394C
    cmplwi r3, 0x7a
    bne @L_80025904
    li r0, 0x0
    b @L_80025910
    @L_80025904:
    li r0, 0x1
    b @L_80025910
    @L_8002590C:
    li r0, 0x0
    @L_80025910:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_80025984
    li r3, 0x2
    bl fn_800D3074
    li r0, 0x0
    li r3, 0x0
    stb r0, lbl_8047A3A8(r13)
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    li r7, 0x0
    bl fn_800EF5FC
    lis r4, 0x8002
    stw r3, lbl_8047A388(r13)
    addi r4, r4, 0x5F74
    li r5, 0x0
    bl fn_800DC390
    b @L_80025960
    @L_8002595C:
    bl fn_800F0308
    @L_80025960:
    lbz r0, lbl_8047A3A8(r13)
    cmplwi r0, 0x0
    beq @L_8002595C
    li r3, 0xc3
    li r4, 0x0
    bl fn_8010264C
    li r3, 0xbd
    bl fn_80102510
    bl fn_80025F84
    @L_80025984:
    mr r3, r30
    li r4, 0x0
    bl fn_80113828
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
