    nofralloc
    mflr r0
    cmpwi r3, 0x0
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    addi r31, r4, 0x0
    ble @L_800A8430
    lwz r0, lbl_8047A838(r13)
    cmpwi r0, 0x1
    beq @L_800A83FC
    bge @L_800A8460
    cmpwi r0, 0x0
    bge @L_800A83D4
    b @L_800A8460
    @L_800A83D4:
    li r0, 0x1
    lwz r4, bb2_8047A83C(r13)
    lis r3, cb@ha
    stw r0, lbl_8047A838(r13)
    addi r7, r3, cb@l
    addi r3, r31, 0x0
    li r5, 0x20
    li r6, 0x420
    bl fn_800A73B4
    b @L_800A8460
    @L_800A83FC:
    li r0, 0x2
    lwz r6, bb2_8047A83C(r13)
    stw r0, lbl_8047A838(r13)
    lis r3, cb@ha
    addi r7, r3, cb@l
    lwz r5, 0x8(r6)
    mr r3, r31
    lwz r4, 0x10(r6)
    addi r0, r5, 0x1f
    lwz r6, 0x4(r6)
    clrrwi r5, r0, 5
    bl fn_800A73B4
    b @L_800A8460
    @L_800A8430:
    cmpwi r3, -0x1
    beq @L_800A8460
    cmpwi r3, -0x4
    bne @L_800A8460
    li r0, 0x0
    stw r0, lbl_8047A838(r13)
    bl DVDReset
    lis r3, cb@ha
    lwz r4, idTmp_8047A840(r13)
    addi r5, r3, cb@l
    addi r3, r31, 0x0
    bl DVDReadDiskID
    @L_800A8460:
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
