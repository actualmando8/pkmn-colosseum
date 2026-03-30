    nofralloc
    cmplwi r3, 0x0
    beq @L_801141A4
    lis r6, lbl_80408378@ha
    addi r6, r6, lbl_80408378@l
    lwz r0, 0x3c(r6)
    stw r0, 0x0(r3)
@L_801141A4:
    cmplwi r4, 0x0
    beq @L_801141BC
    lis r3, lbl_80408378@ha
    addi r3, r3, lbl_80408378@l
    lwz r0, 0x40(r3)
    stw r0, 0x0(r4)
@L_801141BC:
    cmplwi r5, 0x0
    beqlr
    lis r3, lbl_80408378@ha
    addi r3, r3, lbl_80408378@l
    lbz r0, 0x44(r3)
    stb r0, 0x0(r5)
    blr
