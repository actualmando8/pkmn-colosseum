    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    cmpwi r3, 0x0
    bne @L_800B5E0C
    lwz r0, lbl_8047A998(r13)
    cmplwi r0, 0x0
    bne @L_800B5D74
    lwz r3, __memReg(r13)
    addi r6, r3, 0x4e
    lhz r4, 0x0(r6)
    addi r5, r3, 0x50
    b @L_800B5D34
    @L_800B5D34:
    b @L_800B5D38
    @L_800B5D38:
    mr r0, r4
    lhz r4, 0x0(r6)
    lhz r3, 0x0(r5)
    cmplw r4, r0
    bne @L_800B5D38
    slwi r0, r4, 16
    or r0, r0, r3
    stw r0, lbl_8047A988(r13)
    bl OSGetTime
    stw r4, lbl_8047A994(r13)
    li r0, 0x1
    stw r3, lbl_8047A990(r13)
    li r3, 0x0
    stw r0, lbl_8047A998(r13)
    b @L_800B5E78
    @L_800B5D74:
    bl OSGetTime
    lwz r5, __memReg(r13)
    addi r6, r5, 0x4e
    lhz r7, 0x0(r6)
    addi r5, r5, 0x50
    b @L_800B5D8C
    @L_800B5D8C:
    b @L_800B5D90
    @L_800B5D90:
    mr r0, r7
    lhz r7, 0x0(r6)
    lhz r10, 0x0(r5)
    cmplw r7, r0
    bne @L_800B5D90
    lwz r6, lbl_8047A994(r13)
    li r0, 0x0
    lwz r5, lbl_8047A990(r13)
    slwi r9, r7, 16
    subfc r8, r6, r4
    subfe r5, r5, r3
    li r7, 0xa
    xoris r6, r5, 0x8000
    xoris r5, r0, 0x8000
    subfc r0, r7, r8
    subfe r5, r5, r6
    subfe r5, r6, r6
    neg r5, r5
    cmpwi r5, 0x0
    or r5, r9, r10
    beq @L_800B5DEC
    li r3, 0x0
    b @L_800B5E78
    @L_800B5DEC:
    lwz r0, lbl_8047A988(r13)
    cmplw r5, r0
    beq @L_800B5E74
    stw r4, lbl_8047A994(r13)
    stw r3, lbl_8047A990(r13)
    li r3, 0x0
    stw r5, lbl_8047A988(r13)
    b @L_800B5E78
    @L_800B5E0C:
    li r3, 0x0
    bl fn_800B7484
    li r3, 0x0
    bl fn_800B8FD8
    li r3, 0x0
    bl fn_800B90A4
    li r31, 0x0
    lis r3, 0xcc01
    stw r31, -0x8000(r3)
    stw r31, -0x8000(r3)
    stw r31, -0x8000(r3)
    stw r31, -0x8000(r3)
    stw r31, -0x8000(r3)
    stw r31, -0x8000(r3)
    stw r31, -0x8000(r3)
    stw r31, -0x8000(r3)
    bl fn_80098034
    lwz r3, __cpReg(r13)
    li r4, 0x3
    li r0, 0x1
    sth r31, 0x2(r3)
    lwz r3, __cpReg(r13)
    sth r4, 0x4(r3)
    lwz r3, gx(r2)
    stb r0, 0x4f2(r3)
    bl fn_800B897C
    @L_800B5E74:
    li r3, 0x1
    @L_800B5E78:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
