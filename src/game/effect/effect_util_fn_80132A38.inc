    nofralloc
    subi r0, r3, 0xd
    cmplwi r0, 0x50
    bgtlr
    lis r3, jumptable_80363630@ha
    slwi r0, r0, 2
    addi r3, r3, jumptable_80363630@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    clrlwi r0, r4, 16
    sth r0, lbl_8047AE50(r13)
    blr
    clrlwi r0, r4, 16
    sth r0, lbl_8047AE52(r13)
    blr
    stw r4, lbl_8047AE54(r13)
    blr
    stw r4, lbl_8047AE58(r13)
    blr
    stw r4, lbl_8047AE5C(r13)
    blr
    stw r4, lbl_8047AE60(r13)
    blr
    stw r4, lbl_8047AE64(r13)
    blr
    stw r4, lbl_8047AE68(r13)
    blr
    stw r4, lbl_8047AE6C(r13)
    blr
    stw r4, lbl_8047AE70(r13)
    blr
    stw r4, lbl_8047AE74(r13)
    blr
    stw r4, lbl_8047AE78(r13)
    blr
    clrlwi r0, r4, 16
    sth r0, lbl_8047AE7C(r13)
    blr
    stw r4, lbl_8047AE88(r13)
    blr
    stw r4, lbl_8047AE8C(r13)
    blr
    clrlwi r0, r4, 16
    sth r0, lbl_8047AE90(r13)
    blr
    stw r4, lbl_8047AE80(r13)
    blr
    stw r4, lbl_8047AE84(r13)
    blr
    stw r4, lbl_8047AE94(r13)
    blr
    stw r4, lbl_8047AE98(r13)
    blr
    stw r4, lbl_8047AE9C(r13)
    blr
    clrlwi r0, r4, 16
    sth r0, lbl_8047AEA0(r13)
    blr
    clrlwi r0, r4, 16
    sth r0, lbl_8047AEA2(r13)
    blr
    clrlwi r0, r4, 16
    sth r0, lbl_8047AEA4(r13)
    blr
    stw r4, lbl_8047ADC8(r13)
    blr
    stw r4, lbl_8047ADCC(r13)
    blr
    stw r4, lbl_8047ADD0(r13)
    blr
    stw r4, lbl_8047ADD4(r13)
    blr
    stw r4, lbl_8047ADD8(r13)
    blr
    stw r4, lbl_8047ADDC(r13)
    blr
    stw r4, lbl_8047ADE0(r13)
    blr
    stw r4, lbl_8047ADE4(r13)
    blr
    stw r4, lbl_8047ADE8(r13)
    blr
    stw r4, lbl_8047ADEC(r13)
    blr
    stw r4, lbl_8047ADF0(r13)
    blr
    stw r4, lbl_8047ADF4(r13)
    blr
    stw r4, lbl_8047ADF8(r13)
    blr
    stw r4, lbl_8047ADFC(r13)
    blr
    stw r4, lbl_8047AE00(r13)
    blr
    stw r4, lbl_8047AE04(r13)
    blr
    stw r4, lbl_8047AE08(r13)
    blr
    stw r4, lbl_8047AE0C(r13)
    blr
    stw r4, lbl_8047AE10(r13)
    blr
    stw r4, lbl_8047AE14(r13)
    blr
    stw r4, lbl_8047AE18(r13)
    blr
    stw r4, lbl_8047AE1C(r13)
    blr
    stw r4, lbl_8047AE20(r13)
    blr
    stw r4, lbl_8047AE24(r13)
    blr
    stw r4, lbl_8047AE28(r13)
    blr
    stw r4, lbl_8047AE2C(r13)
    blr
    stw r4, lbl_8047AE30(r13)
    blr
    stw r4, lbl_8047AE34(r13)
    blr
    stw r4, lbl_8047AE38(r13)
    blr
    stw r4, lbl_8047AE3C(r13)
    blr
    stw r4, lbl_8047AE40(r13)
    blr
    stw r4, lbl_8047AE44(r13)
    blr
    stw r4, lbl_8047AE48(r13)
    blr
    stw r4, lbl_8047AE4C(r13)
    blr
