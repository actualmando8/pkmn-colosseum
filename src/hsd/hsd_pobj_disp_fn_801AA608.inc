    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, lbl_8036CCD0@ha
    lis r4, lbl_8036C638@ha
    stw r0, 0x14(r1)
    lis r5, lbl_80274EE0@ha
    lis r6, lbl_80274EF8@ha
    addi r3, r3, lbl_8036CCD0@l
    stw r31, 0xc(r1)
    addi r4, r4, lbl_8036C638@l
    addi r5, r5, lbl_80274EE0@l
    addi r6, r6, lbl_80274EF8@l
    li r7, 0x4c
    li r8, 0x1c
    bl fn_80193B30
    lis r8, -32741
    lis r7, -32741
    lis r6, -32741
    lis r3, -32741
    addi r31, r8, -22648
    lis r12, lbl_8036CCD0@ha
    addi r9, r6, -22340
    addi r0, r3, -10632
    lis r10, lbl_8036CCD0@ha
    lis r8, lbl_8036CCD0@ha
    lis r6, lbl_8036CCD0@ha
    lis r3, lbl_8036CCD0@ha
    addi r12, r12, lbl_8036CCD0@l
    addi r8, r8, lbl_8036CCD0@l
    addi r3, r3, lbl_8036CCD0@l
    lis r5, -32741
    addi r11, r7, -22832
    addi r10, r10, lbl_8036CCD0@l
    lis r4, -32741
    addi r7, r5, -21580
    addi r5, r4, -11436
    addi r6, r6, lbl_8036CCD0@l
    lis r4, lbl_8036CCD0@ha
    stw r31, 0x30(r12)
    addi r4, r4, lbl_8036CCD0@l
    stw r11, 0x38(r10)
    stw r9, 0x3c(r8)
    stw r7, 0x40(r6)
    stw r5, 0x44(r4)
    stw r0, 0x48(r3)
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
