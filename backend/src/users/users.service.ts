import { Injectable } from '@nestjs/common';
import { InjectRepository } from '@nestjs/typeorm';
import { Repository } from 'typeorm';
import { User } from './user.entity';

@Injectable()
export class UsersService {
  constructor(
    @InjectRepository(User) private usersRepository: Repository<User>,
  ) {}

  async getAllUsers(): Promise<User[]> {
    return await this.usersRepository.find({});
  }

  async findUserById(id: number): Promise<User> {
    return await this.usersRepository.findOneByOrFail({id});
  }

  async addUser(user: User): Promise<User> {
    return await this.usersRepository.save(user);
  }

  async updateUser(user: User): Promise<User> {
    return await this.usersRepository.save(user);
  }

  async deleteUser(id: number): Promise<void> {
    await this.usersRepository.delete({id});
  }

  async activateUser(id: number): Promise<void> {
    await this.usersRepository.update({id}, {isActive: true});
  }

  async deactivateUser(id: number): Promise<void> {
    await this.usersRepository.update({id}, {isActive: false});
  }

  async userWorking(id: string): Promise<User> {
    const user = await this.usersRepository.findOneByOrFail({cardId: id});
    const isWorkingFinal = user.isWorking;
    const userId = user.id;
    await this.usersRepository.update({id: userId}, {isWorking: !isWorkingFinal, lastAccess: new Date()});
    return user;
  }
}
