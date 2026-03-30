    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    li r27, 0x0
    stw r27, 0x8(r1)
    mr r31, r27
    stw r27, 0xc(r1)
    bl fn_80115BD8
    bl fn_80115B48
    mr r26, r3
    cmplwi r26, 0x0
    beq @L_80111F08
    bl fn_80115BD8
    bl fn_80115A80
    mr r4, r26
    mr r30, r3
    bl fn_800F9318
    lis r4, lbl_802720B0@ha
    addi r4, r4, lbl_802720B0@l
    bl fn_80191ECC
    mr r28, r3
    cmplwi r28, 0x0
    beq @L_80111F08
    lwz r0, 0x0(r28)
    cmplwi r0, 0x0
    beq @L_80111EB0
    mr r3, r26
    bl fn_80115274
    mr r29, r3
    li r25, 0x0
    b @L_80111EA0
@L_80111E78:
    mr r3, r30
    or r4, r29, r27
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_80111E98
    lwz r3, 0x8(r1)
    addi r0, r3, 0x1
    stw r0, 0x8(r1)
@L_80111E98:
    addi r25, r25, 0x4
    addi r27, r27, 0x1
@L_80111EA0:
    lwz r3, 0x0(r28)
    lwzx r0, r3, r25
    cmplwi r0, 0x0
    bne @L_80111E78
@L_80111EB0:
    lwz r0, 0x8(r28)
    cmplwi r0, 0x0
    beq @L_80111F08
    mr r3, r26
    bl fn_80115268
    mr r29, r3
    li r25, 0x0
    b @L_80111EF8
@L_80111ED0:
    mr r3, r30
    or r4, r29, r31
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_80111EF0
    lwz r3, 0xc(r1)
    addi r0, r3, 0x1
    stw r0, 0xc(r1)
@L_80111EF0:
    addi r25, r25, 0x4
    addi r31, r31, 0x1
@L_80111EF8:
    lwz r3, 0x8(r28)
    lwzx r0, r3, r25
    cmplwi r0, 0x0
    bne @L_80111ED0
@L_80111F08:
    lwz r3, 0x8(r1)
    lwz r0, 0xc(r1)
    add r0, r3, r0
    mulli r3, r0, 0x74
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
