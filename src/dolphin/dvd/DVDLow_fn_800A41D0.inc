    nofralloc
    mflr r0
    lis r3, CommandList_803FC290@ha
    stw r0, 0x4(r1)
    addi r4, r3, CommandList_803FC290@l
    stwu r1, -0x8(r1)
    lwz r0, NextCommandNumber_8047A7C4(r13)
    mulli r0, r0, 0x14
    lwzx r3, r4, r0
    cmpwi r3, 0x1
    bne @L_800A4220
    lwz r3, NextCommandNumber_8047A7C4(r13)
    add r6, r4, r0
    addi r0, r3, 0x1
    stw r0, NextCommandNumber_8047A7C4(r13)
    lwz r3, 0x4(r6)
    lwz r4, 0x8(r6)
    lwz r5, 0xc(r6)
    lwz r6, 0x10(r6)
    bl fn_800A42C4
    b @L_800A4244
    @L_800A4220:
    cmpwi r3, 0x2
    bne @L_800A4244
    lwz r3, NextCommandNumber_8047A7C4(r13)
    add r4, r4, r0
    addi r0, r3, 0x1
    stw r0, NextCommandNumber_8047A7C4(r13)
    lwz r3, 0xc(r4)
    lwz r4, 0x10(r4)
    bl fn_800A46EC
    @L_800A4244:
    lwz r0, 0xc(r1)
    addi r1, r1, 0x8
    mtlr r0
    blr
