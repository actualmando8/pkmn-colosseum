    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stfd f31, 0x20(r1)
    psq_st f31, 0x28(r1), 0, 0
    stmw r27, 0xc(r1)
    mr r28, r3
    li r30, 0x3
    addi r31, r28, 0xe4
    @L_800EDABC:
    lwz r0, 0x0(r31)
    cmpwi r0, 0x0
    beq @L_800EE018
    lwz r4, 0x4(r31)
    mr r3, r28
    bl fn_800EE150
    mr. r29, r3
    beq @L_800EE018
    lwz r0, 0x0(r31)
    cmpwi r0, 0x2
    beq @L_800EDCDC
    bge @L_800EDAF8
    cmpwi r0, 0x1
    bge @L_800EDB04
    b @L_800EE010
    @L_800EDAF8:
    cmpwi r0, 0x4
    bge @L_800EE010
    b @L_800EDE6C
    @L_800EDB04:
    lwz r27, 0x8(r29)
    lwz r3, 0x8(r31)
    cmplwi r27, 0x0
    lfs f31, 0x0(r3)
    bne @L_800EDB28
    la r3, lbl_8047CC68(r2)
    li r4, 0x2a4
    la r5, lbl_8047CC70(r2)
    bl fn_80196E10
    @L_800EDB28:
    lwz r0, 0x14(r27)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800EDB48
    lis r4, lbl_80270EE8@ha
    la r3, lbl_8047CC68(r2)
    addi r5, r4, lbl_80270EE8@l
    li r4, 0x2a5
    bl fn_80196E10
    @L_800EDB48:
    stfs f31, 0x1c(r27)
    lwz r0, 0x14(r27)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800EDBA0
    cmplwi r27, 0x0
    beq @L_800EDBA0
    bne @L_800EDB74
    la r3, lbl_8047CC68(r2)
    li r4, 0x25d
    la r5, lbl_8047CC70(r2)
    bl fn_80196E10
    @L_800EDB74:
    lwz r4, 0x14(r27)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800EDB90
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800EDB90
    li r3, 0x1
    @L_800EDB90:
    cmpwi r3, 0x0
    bne @L_800EDBA0
    mr r3, r27
    bl fn_8019D620
    @L_800EDBA0:
    lwz r27, 0x8(r29)
    lwz r3, 0x8(r31)
    cmplwi r27, 0x0
    lfs f31, 0x4(r3)
    bne @L_800EDBC4
    la r3, lbl_8047CC68(r2)
    li r4, 0x2b8
    la r5, lbl_8047CC70(r2)
    bl fn_80196E10
    @L_800EDBC4:
    lwz r0, 0x14(r27)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800EDBE4
    lis r4, lbl_80270EE8@ha
    la r3, lbl_8047CC68(r2)
    addi r5, r4, lbl_80270EE8@l
    li r4, 0x2b9
    bl fn_80196E10
    @L_800EDBE4:
    stfs f31, 0x20(r27)
    lwz r0, 0x14(r27)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800EDC3C
    cmplwi r27, 0x0
    beq @L_800EDC3C
    bne @L_800EDC10
    la r3, lbl_8047CC68(r2)
    li r4, 0x25d
    la r5, lbl_8047CC70(r2)
    bl fn_80196E10
    @L_800EDC10:
    lwz r4, 0x14(r27)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800EDC2C
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800EDC2C
    li r3, 0x1
    @L_800EDC2C:
    cmpwi r3, 0x0
    bne @L_800EDC3C
    mr r3, r27
    bl fn_8019D620
    @L_800EDC3C:
    lwz r27, 0x8(r29)
    lwz r3, 0x8(r31)
    cmplwi r27, 0x0
    lfs f31, 0x8(r3)
    bne @L_800EDC60
    la r3, lbl_8047CC68(r2)
    li r4, 0x2cc
    la r5, lbl_8047CC70(r2)
    bl fn_80196E10
    @L_800EDC60:
    lwz r0, 0x14(r27)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800EDC80
    lis r4, lbl_80270EE8@ha
    la r3, lbl_8047CC68(r2)
    addi r5, r4, lbl_80270EE8@l
    li r4, 0x2cd
    bl fn_80196E10
    @L_800EDC80:
    stfs f31, 0x24(r27)
    lwz r0, 0x14(r27)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800EE010
    cmplwi r27, 0x0
    beq @L_800EE010
    bne @L_800EDCAC
    la r3, lbl_8047CC68(r2)
    li r4, 0x25d
    la r5, lbl_8047CC70(r2)
    bl fn_80196E10
    @L_800EDCAC:
    lwz r4, 0x14(r27)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800EDCC8
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800EDCC8
    li r3, 0x1
    @L_800EDCC8:
    cmpwi r3, 0x0
    bne @L_800EE010
    mr r3, r27
    bl fn_8019D620
    b @L_800EE010
    @L_800EDCDC:
    lwz r27, 0x8(r29)
    lwz r3, 0x8(r31)
    cmplwi r27, 0x0
    lfs f31, 0x0(r3)
    bne @L_800EDD00
    la r3, lbl_8047CC68(r2)
    li r4, 0x412
    la r5, lbl_8047CC70(r2)
    bl fn_80196E10
    @L_800EDD00:
    lfs f0, 0x1c(r27)
    fadds f0, f0, f31
    stfs f0, 0x1c(r27)
    lwz r0, 0x14(r27)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800EDD60
    cmplwi r27, 0x0
    beq @L_800EDD60
    bne @L_800EDD34
    la r3, lbl_8047CC68(r2)
    li r4, 0x25d
    la r5, lbl_8047CC70(r2)
    bl fn_80196E10
    @L_800EDD34:
    lwz r4, 0x14(r27)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800EDD50
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800EDD50
    li r3, 0x1
    @L_800EDD50:
    cmpwi r3, 0x0
    bne @L_800EDD60
    mr r3, r27
    bl fn_8019D620
    @L_800EDD60:
    lwz r27, 0x8(r29)
    lwz r3, 0x8(r31)
    cmplwi r27, 0x0
    lfs f31, 0x4(r3)
    bne @L_800EDD84
    la r3, lbl_8047CC68(r2)
    li r4, 0x41d
    la r5, lbl_8047CC70(r2)
    bl fn_80196E10
    @L_800EDD84:
    lfs f0, 0x20(r27)
    fadds f0, f0, f31
    stfs f0, 0x20(r27)
    lwz r0, 0x14(r27)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800EDDE4
    cmplwi r27, 0x0
    beq @L_800EDDE4
    bne @L_800EDDB8
    la r3, lbl_8047CC68(r2)
    li r4, 0x25d
    la r5, lbl_8047CC70(r2)
    bl fn_80196E10
    @L_800EDDB8:
    lwz r4, 0x14(r27)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800EDDD4
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800EDDD4
    li r3, 0x1
    @L_800EDDD4:
    cmpwi r3, 0x0
    bne @L_800EDDE4
    mr r3, r27
    bl fn_8019D620
    @L_800EDDE4:
    lwz r27, 0x8(r29)
    lwz r3, 0x8(r31)
    cmplwi r27, 0x0
    lfs f31, 0x8(r3)
    bne @L_800EDE08
    la r3, lbl_8047CC68(r2)
    li r4, 0x428
    la r5, lbl_8047CC70(r2)
    bl fn_80196E10
    @L_800EDE08:
    lfs f0, 0x24(r27)
    fadds f0, f0, f31
    stfs f0, 0x24(r27)
    lwz r0, 0x14(r27)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800EE010
    cmplwi r27, 0x0
    beq @L_800EE010
    bne @L_800EDE3C
    la r3, lbl_8047CC68(r2)
    li r4, 0x25d
    la r5, lbl_8047CC70(r2)
    bl fn_80196E10
    @L_800EDE3C:
    lwz r4, 0x14(r27)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800EDE58
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800EDE58
    li r3, 0x1
    @L_800EDE58:
    cmpwi r3, 0x0
    bne @L_800EE010
    mr r3, r27
    bl fn_8019D620
    b @L_800EE010
    @L_800EDE6C:
    lwz r27, 0x8(r29)
    lwz r3, 0x8(r31)
    cmplwi r27, 0x0
    lfs f31, 0x0(r3)
    bne @L_800EDE90
    la r3, lbl_8047CC68(r2)
    li r4, 0x412
    la r5, lbl_8047CC70(r2)
    bl fn_80196E10
    @L_800EDE90:
    lfs f0, 0x1c(r27)
    fadds f0, f0, f31
    stfs f0, 0x1c(r27)
    lwz r0, 0x14(r27)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800EDEF0
    cmplwi r27, 0x0
    beq @L_800EDEF0
    bne @L_800EDEC4
    la r3, lbl_8047CC68(r2)
    li r4, 0x25d
    la r5, lbl_8047CC70(r2)
    bl fn_80196E10
    @L_800EDEC4:
    lwz r4, 0x14(r27)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800EDEE0
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800EDEE0
    li r3, 0x1
    @L_800EDEE0:
    cmpwi r3, 0x0
    bne @L_800EDEF0
    mr r3, r27
    bl fn_8019D620
    @L_800EDEF0:
    lwz r27, 0x8(r29)
    lwz r3, 0x8(r31)
    cmplwi r27, 0x0
    lfs f31, 0x4(r3)
    bne @L_800EDF14
    la r3, lbl_8047CC68(r2)
    li r4, 0x2b8
    la r5, lbl_8047CC70(r2)
    bl fn_80196E10
    @L_800EDF14:
    lwz r0, 0x14(r27)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800EDF34
    lis r4, lbl_80270EE8@ha
    la r3, lbl_8047CC68(r2)
    addi r5, r4, lbl_80270EE8@l
    li r4, 0x2b9
    bl fn_80196E10
    @L_800EDF34:
    stfs f31, 0x20(r27)
    lwz r0, 0x14(r27)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800EDF8C
    cmplwi r27, 0x0
    beq @L_800EDF8C
    bne @L_800EDF60
    la r3, lbl_8047CC68(r2)
    li r4, 0x25d
    la r5, lbl_8047CC70(r2)
    bl fn_80196E10
    @L_800EDF60:
    lwz r4, 0x14(r27)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800EDF7C
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800EDF7C
    li r3, 0x1
    @L_800EDF7C:
    cmpwi r3, 0x0
    bne @L_800EDF8C
    mr r3, r27
    bl fn_8019D620
    @L_800EDF8C:
    lwz r27, 0x8(r29)
    lwz r3, 0x8(r31)
    cmplwi r27, 0x0
    lfs f31, 0x8(r3)
    bne @L_800EDFB0
    la r3, lbl_8047CC68(r2)
    li r4, 0x428
    la r5, lbl_8047CC70(r2)
    bl fn_80196E10
    @L_800EDFB0:
    lfs f0, 0x24(r27)
    fadds f0, f0, f31
    stfs f0, 0x24(r27)
    lwz r0, 0x14(r27)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800EE010
    cmplwi r27, 0x0
    beq @L_800EE010
    bne @L_800EDFE4
    la r3, lbl_8047CC68(r2)
    li r4, 0x25d
    la r5, lbl_8047CC70(r2)
    bl fn_80196E10
    @L_800EDFE4:
    lwz r4, 0x14(r27)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800EE000
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800EE000
    li r3, 0x1
    @L_800EE000:
    cmpwi r3, 0x0
    bne @L_800EE010
    mr r3, r27
    bl fn_8019D620
    @L_800EE010:
    mr r3, r29
    bl fn_800EE828
    @L_800EE018:
    cmpwi r30, 0x0
    addi r31, r31, 0xc
    subi r30, r30, 0x1
    bne @L_800EDABC
    psq_l f31, 0x28(r1), 0, 0
    lfd f31, 0x20(r1)
    lmw r27, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
