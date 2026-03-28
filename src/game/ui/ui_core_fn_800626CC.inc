    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r4
    cmplwi r27, 0x0
    li r30, 0x1
    beq @L_800626F8
    lwz r3, 0x4(r27)
    bl fn_800F92D4
    stw r3, 0x0(r27)
@L_800626F8:
    lis r3, lbl_803A9A60@ha
    addi r31, r3, lbl_803A9A60@l
@L_80062700:
    lwz r3, 0x2c(r31)
    lwz r0, 0x30(r31)
    cmpw r3, r0
    bne @L_80062718
    li r30, 0x0
    b @L_80062818
@L_80062718:
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80062768
    lwz r0, 0x2c(r31)
    srwi r3, r0, 31
    clrlwi r0, r0, 31
    xor r0, r0, r3
    subf r0, r3, r0
    cmpwi r0, 0x0
    beq @L_80062754
    li r3, 0x1
    li r4, 0x0
    bl fn_8025D364
    mr r28, r3
    b @L_80062790
@L_80062754:
    li r3, 0x0
    li r4, 0x1
    bl fn_8025D364
    mr r28, r3
    b @L_80062790
@L_80062768:
    lwz r3, 0x2c(r31)
    cmpwi r3, 0x2
    bge @L_80062784
    li r4, 0x1
    bl fn_8025D2D4
    mr r28, r3
    b @L_80062790
@L_80062784:
    li r4, 0x0
    bl fn_8025D2D4
    mr r28, r3
@L_80062790:
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_800627A4
    li r27, 0x5c3
    b @L_800627A8
@L_800627A4:
    li r27, 0x5c4
@L_800627A8:
    lwz r4, 0x2c(r31)
    lis r3, lbl_803A9A60@ha
    addi r0, r3, lbl_803A9A60@l
    slwi r3, r4, 3
    add r3, r0, r3
    addi r29, r3, 0xc
    stw r28, 0x4(r29)
    lwz r3, 0x4(r29)
    bl fn_800F92D4
    stw r3, 0x0(r29)
    lwz r0, 0x0(r29)
    cmplwi r0, 0x0
    beq @L_800627EC
    lwz r3, 0x2c(r31)
    addi r0, r3, 0x1
    stw r0, 0x2c(r31)
    b @L_80062818
@L_800627EC:
    lis r4, fn_800626CC@ha
    mr r3, r27
    addi r5, r4, fn_800626CC@l
    mr r6, r29
    mr r4, r28
    mr r7, r28
    bl fn_8017B000
    lwz r3, 0x2c(r31)
    li r30, 0x0
    addi r0, r3, 0x1
    stw r0, 0x2c(r31)
@L_80062818:
    cmpwi r30, 0x0
    bne @L_80062700
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
