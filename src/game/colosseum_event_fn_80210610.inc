    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r3, r4
    stw r0, 0x14(r1)
    bl fn_80203E0C
    rlwinm r3, r3, 0, 24, 31
    cmplwi r3, 0x21
    bge @L_80210638
    li r3, 0x0
    b @L_80210648
@L_80210638:
    li r0, 0x42
    subfc r0, r0, r3
    subfe r3, r0, r0
    addi r3, r3, 0x2
@L_80210648:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
