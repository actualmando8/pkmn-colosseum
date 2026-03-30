    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    lis r30, lbl_80404ACC@ha
    addi r30, r30, lbl_80404ACC@l
    rlwinm r31, r4, 0, 24, 31
@L_801045D0:
    cmpwi r29, 0x0
    bgt @L_801045E0
    li r3, 0x0
    b @L_80104608
@L_801045E0:
    lwz r3, 0xc(r30)
    b @L_801045FC
@L_801045E8:
    lwz r0, 0x4(r3)
    cmpw r0, r29
    bne @L_801045F8
    b @L_80104608
@L_801045F8:
    lwz r3, 0x10(r3)
@L_801045FC:
    cmplwi r3, 0x0
    bne @L_801045E8
    li r3, 0x0
@L_80104608:
    cmplwi r3, 0x0
    bne @L_80104634
    lis r3, lbl_80271E40@ha
    lis r4, lbl_8035B070@ha
    addi r3, r3, lbl_80271E40@l
    mr r5, r29
    addi r4, r4, lbl_8035B070@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_8010469C
@L_80104634:
    lbz r0, 0x98(r3)
    cmplwi r0, 0x0
    beq @L_80104648
    li r3, 0x0
    b @L_8010469C
@L_80104648:
    lbz r0, 0x99(r3)
    cmplwi r0, 0x0
    beq @L_8010465C
    li r3, 0x0
    b @L_8010469C
@L_8010465C:
    cmplwi r31, 0x0
    beq @L_80104698
    bl fn_800F037C
    cmplwi r3, 0x0
    bne @L_80104690
    lis r3, lbl_80271E64@ha
    lis r4, lbl_8035B070@ha
    addi r3, r3, lbl_80271E64@l
    mr r5, r29
    addi r4, r4, lbl_8035B070@l
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_80104698
@L_80104690:
    bl fn_800F0308
    b @L_801045D0
@L_80104698:
    li r3, 0x1
@L_8010469C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
