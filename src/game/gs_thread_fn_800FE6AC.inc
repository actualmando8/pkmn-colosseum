    nofralloc
    cmplwi r3, 0x0
    beq @L_800FE6BC
    lha r0, lbl_8047AC70(r13)
    sth r0, 0x0(r3)
@L_800FE6BC:
    cmplwi r4, 0x0
    beqlr
    lha r0, lbl_8047AC72(r13)
    sth r0, 0x0(r4)
    blr
