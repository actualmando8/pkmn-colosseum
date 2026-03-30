    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    lbz r0, 0x1(r30)
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_8006EEA8
    b @L_8006EF04
    @L_8006EEA8:
    bl fn_80105624
    lhz r0, 0x4(r3)
    rlwinm r0, r0, 0, 27, 27
    cmpwi r0, 0x0
    beq @L_8006EF04
    lbz r3, 0x95(r30)
    lbz r0, 0x94(r30)
    extsb r3, r3
    extsb r0, r0
    add r31, r3, r0
    cmpwi r31, 0x3c
    bge @L_8006EF04
    mr r3, r30
    li r4, 0x0
    bl fn_801040D0
    lbzx r4, r3, r31
    li r0, 0x0
    cntlzw r4, r4
    srwi r4, r4, 5
    clrlwi r4, r4, 24
    stbx r4, r3, r31
    stb r0, 0x98(r30)
    b @L_8006EF0C
    @L_8006EF04:
    mr r3, r30
    bl fn_80102ED4
    @L_8006EF0C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
