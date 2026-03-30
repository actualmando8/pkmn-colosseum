    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_80115C48
    cmplwi r3, 0x0
    bne @L_80113FD4
    li r3, 0x0
    b @L_80113FD8
@L_80113FD4:
    bl fn_80115A80
@L_80113FD8:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
