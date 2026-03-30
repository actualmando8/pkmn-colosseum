    nofralloc
    lwz r5, -29632(r2)
    cmpwi r4, 0x1
    stw r4, 0x420(r5)
    lfs f0, 0x0(r3)
    stfs f0, 0x424(r5)
    lfs f0, 0x14(r3)
    stfs f0, 0x42c(r5)
    lfs f0, 0x28(r3)
    stfs f0, 0x434(r5)
    lfs f0, 0x2c(r3)
    stfs f0, 0x438(r5)
    bne @L_800BD324
    lfs f0, 0xc(r3)
    stfs f0, 0x428(r5)
    lfs f0, 0x1c(r3)
    stfs f0, 0x430(r5)
    b @L_800BD334
@L_800BD324:
    lfs f0, 0x8(r3)
    stfs f0, 0x428(r5)
    lfs f0, 0x18(r3)
    stfs f0, 0x430(r5)
@L_800BD334:
    li r0, 0x10
    lwz r4, -29632(r2)
    lis r5, 0xcc01
    lis r3, 0x6
    stb r0, -0x8000(r5)
    addi r0, r3, 0x1020
    stw r0, -0x8000(r5)
    li r0, 0x1
    lfs f0, 0x424(r4)
    stfs f0, -0x8000(r5)
    lfs f0, 0x428(r4)
    stfs f0, -0x8000(r5)
    lfs f0, 0x42c(r4)
    stfs f0, -0x8000(r5)
    lfs f0, 0x430(r4)
    stfs f0, -0x8000(r5)
    lfs f0, 0x434(r4)
    stfs f0, -0x8000(r5)
    lfs f0, 0x438(r4)
    stfs f0, -0x8000(r5)
    lwz r3, 0x420(r4)
    stw r3, -0x8000(r5)
    sth r0, 0x2(r4)
    blr
