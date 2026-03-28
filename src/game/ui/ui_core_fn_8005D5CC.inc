    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    lis r5, lbl_80267AB0@ha
    li r0, 0x9
    addi r5, r5, lbl_80267AB0@l
    addi r7, r1, 0x4
    subi r6, r5, 0x4
    mtctr r0
@L_8005D5F0:
    lwz r5, 0x4(r6)
    lwzu r0, 0x8(r6)
    stw r5, 0x4(r7)
    stwu r0, 0x8(r7)
    bdnz @L_8005D5F0
    addi r6, r1, 0x8
    li r8, 0x0
    li r0, 0x6
    mtctr r0
@L_8005D614:
    li r7, 0x0
    lha r5, 0x6(r4)
    lwz r0, 0x0(r6)
    cmpw r5, r0
    beq @L_8005D64C
    li r7, 0x1
    lwz r0, 0x4(r6)
    cmpw r5, r0
    beq @L_8005D64C
    li r7, 0x2
    lwz r0, 0x8(r6)
    cmpw r5, r0
    beq @L_8005D64C
    li r7, 0x3
@L_8005D64C:
    cmpwi r7, 0x3
    blt @L_8005D660
    addi r6, r6, 0xc
    addi r8, r8, 0x1
    bdnz @L_8005D614
@L_8005D660:
    cmpwi r8, 0x6
    blt @L_8005D670
    li r3, 0x0
    b @L_8005D698
@L_8005D670:
    lbz r0, 0x95(r3)
    mr r3, r4
    extsb r0, r0
    cmpw r0, r8
    bne @L_8005D68C
    li r4, 0x1
    b @L_8005D690
@L_8005D68C:
    li r4, 0x0
@L_8005D690:
    bl fn_80109220
    li r3, 0x0
@L_8005D698:
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
