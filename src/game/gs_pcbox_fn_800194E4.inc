    nofralloc
    lha r0, 0x6(r4)
    lwz r3, 0x60(r3)
    cmpwi r0, 0x26c
    beq @L_800194FC
    lwz r0, 0x0(r3)
    stw r0, 0x64(r4)
    @L_800194FC:
    lwz r7, 0xc(r3)
    cmpwi r7, 0x4
    bge @L_800195D8
    lis r3, lbl_802E4E10@ha
    li r6, 0x0
    addi r5, r3, lbl_802E4E10@l
    lha r3, 0x6(r4)
    lwz r0, 0x0(r5)
    cmpw r3, r0
    beq @L_8001958C
    addi r5, r5, 0xc
    li r6, 0x1
    lwz r0, 0x0(r5)
    cmpw r3, r0
    beq @L_8001958C
    addi r5, r5, 0xc
    li r6, 0x2
    lwz r0, 0x0(r5)
    cmpw r3, r0
    beq @L_8001958C
    addi r5, r5, 0xc
    li r6, 0x3
    lwz r0, 0x0(r5)
    cmpw r3, r0
    beq @L_8001958C
    addi r5, r5, 0xc
    li r6, 0x4
    lwz r0, 0x0(r5)
    cmpw r3, r0
    beq @L_8001958C
    addi r5, r5, 0xc
    li r6, 0x5
    lwz r0, 0x0(r5)
    cmpw r3, r0
    beq @L_8001958C
    li r6, 0x6
    @L_8001958C:
    cmpwi r6, 0x6
    bge @L_800195D8
    mulli r6, r6, 0xc
    lis r3, lbl_802E4E10@ha
    subfic r5, r7, 0x4
    addi r0, r3, lbl_802E4E10@l
    add r3, r0, r6
    mulli r5, r5, 0x1f
    lha r0, 0x4(r3)
    add r0, r5, r0
    extsh r0, r0
    sth r0, 0x52(r4)
    lwz r0, 0x8(r3)
    cmpwi r0, 0x0
    beq @L_800195D8
    lha r0, 0x6(r3)
    subf r0, r5, r0
    extsh r0, r0
    sth r0, 0x56(r4)
    @L_800195D8:
    li r3, 0x0
    blr
