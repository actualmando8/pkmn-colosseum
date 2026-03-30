    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    addi r31, r4, 0x0
    stw r30, 0x10(r1)
    addi r30, r3, 0x0
    lwz r5, -29632(r2)
    lwz r0, 0x4f4(r5)
    cmplwi r0, 0x0
    beq @L_800BD12C
    bl fn_800B91EC
@L_800BD12C:
    lwz r3, -29632(r2)
    lwz r0, 0x0(r3)
    cmplwi r0, 0x0
    bne @L_800BD140
    bl fn_800B937C
@L_800BD140:
    li r0, 0x40
    lis r3, 0xcc01
    stb r0, -0x8000(r3)
    stw r30, -0x8000(r3)
    stw r31, -0x8000(r3)
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
