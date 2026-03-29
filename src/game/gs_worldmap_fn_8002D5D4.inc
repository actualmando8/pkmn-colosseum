    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    lwz r3, lbl_8047A3FC(r13)
    lwz r4, lbl_80478E54(r13)
    slwi r0, r3, 2
    mr r31, r3
    add r4, r4, r0
    lbz r30, 0x1(r4)
    cmpwi r30, 0x1
    beq @L_8002D740
    bge @L_8002D74C
    cmpwi r30, 0x0
    bge @L_8002D614
    b @L_8002D74C
    @L_8002D614:
    li r3, 0x0
    li r4, 0xc
    li r5, 0x0
    bl fn_8012A5B0
    mr r7, r3
    mr r4, r31
    addi r3, r1, 0x9
    li r5, 0x0
    li r6, 0x4b
    li r8, -0x1
    crxor 6, 6, 6
    bl fn_8002A0B8
    lbz r7, 0x9(r1)
    mr r4, r3
    li r3, 0x2
    li r5, 0x1
    li r6, 0x0
    bl fn_80106ADC
    b @L_8002D6D0
    @L_8002D660:
    li r3, 0x1
    bl fn_801069FC
    cmpwi r27, 0x1
    beq @L_8002D690
    bge @L_8002D6A0
    cmpwi r27, 0x0
    bge @L_8002D680
    b @L_8002D6A0
    @L_8002D680:
    mr r3, r31
    mr r4, r30
    bl fn_8002D154
    b @L_8002D6A0
    @L_8002D690:
    mr r4, r31
    li r3, 0x3
    li r5, 0x0
    bl fn_80018F54
    @L_8002D6A0:
    mr r4, r31
    addi r3, r1, 0x9
    li r5, 0x1
    li r6, -0x1
    crxor 6, 6, 6
    bl fn_8002A0B8
    lbz r7, 0x9(r1)
    mr r4, r3
    li r3, 0x2
    li r5, 0x1
    li r6, 0x0
    bl fn_80106ADC
    @L_8002D6D0:
    li r3, 0x62
    li r4, 0x1
    bl fn_8010264C
    mr r29, r3
    li r3, 0x62
    bl fn_80102510
    li r3, 0x62
    li r4, 0x1
    bl fn_80102428
    cmpwi r29, -0x1
    beq @L_8002D704
    cmpwi r29, 0x2
    bne @L_8002D70C
    @L_8002D704:
    li r27, 0x2
    b @L_8002D720
    @L_8002D70C:
    cmpwi r29, 0x0
    bne @L_8002D71C
    li r27, 0x0
    b @L_8002D720
    @L_8002D71C:
    li r27, 0x1
    @L_8002D720:
    cmpwi r27, 0x2
    bne @L_8002D660
    mr r3, r31
    li r4, 0x2
    li r5, -0x1
    crxor 6, 6, 6
    bl fn_8002A2CC
    b @L_8002D8E8
    @L_8002D740:
    mr r4, r30
    bl fn_8002CE6C
    b @L_8002D8E8
    @L_8002D74C:
    li r27, 0x0
    li r3, 0x7198
    bl fn_800E3534
    mr r28, r3
    bl fn_800E27B0
    stw r3, lbl_8047A3DC(r13)
    mr r4, r31
    addi r3, r1, 0x8
    li r5, 0x0
    li r6, -0x1
    crxor 6, 6, 6
    bl fn_80029FAC
    lbz r7, 0x8(r1)
    mr r4, r3
    li r3, 0x2
    li r5, 0x1
    li r6, 0x0
    bl fn_80106ADC
    b @L_8002D83C
    @L_8002D798:
    li r3, 0x1
    bl fn_801069FC
    cmpwi r29, 0x2
    beq @L_8002D7E8
    bge @L_8002D7BC
    cmpwi r29, 0x0
    beq @L_8002D7C8
    bge @L_8002D7D8
    b @L_8002D804
    @L_8002D7BC:
    cmpwi r29, 0x4
    bge @L_8002D804
    b @L_8002D800
    @L_8002D7C8:
    mr r3, r31
    mr r4, r30
    bl fn_8002C408
    b @L_8002D804
    @L_8002D7D8:
    mr r3, r31
    mr r4, r30
    bl fn_8002C284
    b @L_8002D804
    @L_8002D7E8:
    mr r3, r31
    li r4, 0xb
    li r5, -0x1
    crxor 6, 6, 6
    bl fn_8002A1C4
    b @L_8002D804
    @L_8002D800:
    li r27, 0x1
    @L_8002D804:
    cmpwi r27, 0x0
    bne @L_8002D8A8
    mr r4, r31
    addi r3, r1, 0x8
    li r5, 0x1
    li r6, -0x1
    crxor 6, 6, 6
    bl fn_80029FAC
    lbz r7, 0x8(r1)
    mr r4, r3
    li r3, 0x2
    li r5, 0x1
    li r6, 0x0
    bl fn_80106ADC
    @L_8002D83C:
    li r3, 0x83
    li r4, 0x1
    bl fn_8010264C
    mr r29, r3
    li r3, 0x83
    bl fn_80102510
    li r3, 0x83
    li r4, 0x1
    bl fn_80102428
    cmpwi r29, 0x1
    beq @L_8002D88C
    bge @L_8002D878
    cmpwi r29, 0x0
    bge @L_8002D884
    b @L_8002D89C
    @L_8002D878:
    cmpwi r29, 0x3
    bge @L_8002D89C
    b @L_8002D894
    @L_8002D884:
    li r29, 0x0
    b @L_8002D8A0
    @L_8002D88C:
    li r29, 0x1
    b @L_8002D8A0
    @L_8002D894:
    li r29, 0x2
    b @L_8002D8A0
    @L_8002D89C:
    li r29, 0x3
    @L_8002D8A0:
    cmpwi r29, 0x3
    bne @L_8002D798
    @L_8002D8A8:
    cmplwi r30, 0x3
    beq @L_8002D8D0
    cmplwi r30, 0x2
    beq @L_8002D8D0
    mr r3, r31
    li r4, 0x2
    li r5, -0x1
    crxor 6, 6, 6
    bl fn_8002A1C4
    b @L_8002D8D8
    @L_8002D8D0:
    li r3, 0x1
    bl fn_801069FC
    @L_8002D8D8:
    mr r3, r28
    bl fn_800E24B0
    mr r3, r28
    bl fn_800E209C
    @L_8002D8E8:
    la r3, lbl_8047A3FC(r13)
    lwz r0, 0x4(r3)
    cmpwi r0, 0x0
    beq @L_8002D908
    bl fn_800FF660
    li r3, 0x0
    li r4, 0x0
    bl fn_8011288C
    @L_8002D908:
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
