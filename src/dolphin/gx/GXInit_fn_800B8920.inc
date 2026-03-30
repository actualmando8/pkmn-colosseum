    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x8(r1)
    lwz r3, -29632(r2)
    lwz r0, 0x4f4(r3)
    cmplwi r0, 0x0
    beq @L_800B8940
    bl fn_800B91EC
@L_800B8940:
    li r0, 0x0
    lis r3, 0xcc01
    stw r0, -0x8000(r3)
    stw r0, -0x8000(r3)
    stw r0, -0x8000(r3)
    stw r0, -0x8000(r3)
    stw r0, -0x8000(r3)
    stw r0, -0x8000(r3)
    stw r0, -0x8000(r3)
    stw r0, -0x8000(r3)
    bl fn_80098034
    lwz r0, 0xc(r1)
    addi r1, r1, 0x8
    mtlr r0
    blr
