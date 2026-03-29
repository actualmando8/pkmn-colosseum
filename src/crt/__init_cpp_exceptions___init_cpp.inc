    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x10(r1)
    stw r31, 0xc(r1)
    lis r3, _ctors@ha
    addi r0, r3, _ctors@l
    mr r31, r0
    b @L_800A2BDC
@L_800A2BDC:
    b @L_800A2BE0
@L_800A2BE0:
    b @L_800A2BF0
@L_800A2BE4:
    mtlr r12
    blrl
    addi r31, r31, 0x4
@L_800A2BF0:
    lwz r12, 0x0(r31)
    cmplwi r12, 0x0
    bne @L_800A2BE4
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    addi r1, r1, 0x10
    mtlr r0
    blr
