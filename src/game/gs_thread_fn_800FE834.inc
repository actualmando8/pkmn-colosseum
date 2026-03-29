    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmpwi r3, 0x2
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bne @L_800FE864
    lwz r0, lbl_8047AC80(r13)
    lwz r7, lbl_8047AC7C(r13)
    mulli r0, r0, 0x18
    lwz r8, lbl_8047AC84(r13)
    add r31, r7, r0
    b @L_800FE86C
@L_800FE864:
    lwz r31, lbl_8047AC7C(r13)
    lwz r8, lbl_8047AC80(r13)
@L_800FE86C:
    mtctr r8
    cmplwi r8, 0x0
    beq @L_800FE890
@L_800FE878:
    lwz r0, 0x8(r31)
    cmpwi r0, 0x0
    bne @L_800FE888
    b @L_800FE894
@L_800FE888:
    addi r31, r31, 0x18
    bdnz @L_800FE878
@L_800FE890:
    li r31, 0x0
@L_800FE894:
    cmplwi r31, 0x0
    bne @L_800FE8A4
    li r3, 0x0
    b @L_800FE99C
@L_800FE8A4:
    li r0, 0x0
    stw r0, 0x0(r31)
    stw r0, 0x4(r31)
    stw r3, 0x8(r31)
    stb r4, 0xc(r31)
    stb r0, 0xd(r31)
    stw r5, 0x10(r31)
    stw r6, 0x14(r31)
    lwz r0, lbl_8047AC98(r13)
    cmplwi r0, 0x0
    bne @L_800FE8D8
    stw r31, lbl_8047AC98(r13)
    b @L_800FE980
@L_800FE8D8:
    bl OSDisableInterrupts
    lwz r0, 0x8(r31)
    cmpwi r0, 0x2
    bne @L_800FE8F8
    lwz r0, lbl_8047AC9C(r13)
    stw r0, 0x4(r31)
    stw r31, lbl_8047AC9C(r13)
    b @L_800FE97C
@L_800FE8F8:
    lwz r5, lbl_8047AC98(r13)
    b @L_800FE904
@L_800FE900:
    mr r5, r6
@L_800FE904:
    lwz r6, 0x4(r5)
    cmplwi r6, 0x0
    beq @L_800FE920
    lbz r4, 0xc(r5)
    lbz r0, 0xc(r31)
    cmplw r4, r0
    blt @L_800FE900
@L_800FE920:
    cmplwi r6, 0x0
    bne @L_800FE94C
    lbz r4, 0xc(r5)
    lbz r0, 0xc(r31)
    cmplw r4, r0
    bge @L_800FE94C
    stw r5, 0x0(r31)
    li r0, 0x0
    stw r0, 0x4(r31)
    stw r31, 0x4(r5)
    b @L_800FE97C
@L_800FE94C:
    lwz r4, 0x0(r5)
    cmplwi r4, 0x0
    beq @L_800FE95C
    stw r31, 0x4(r4)
@L_800FE95C:
    lwz r0, 0x0(r5)
    stw r0, 0x0(r31)
    stw r5, 0x4(r31)
    stw r31, 0x0(r5)
    lwz r0, lbl_8047AC98(r13)
    cmplw r0, r5
    bne @L_800FE97C
    stw r31, lbl_8047AC98(r13)
@L_800FE97C:
    bl OSRestoreInterrupts
@L_800FE980:
    lwz r0, lbl_8047AC7C(r13)
    lis r3, 0xaaab
    subi r3, r3, 0x5555
    subf r0, r0, r31
    mulhwu r0, r3, r0
    srwi r3, r0, 4
    addi r3, r3, 0x1
@L_800FE99C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
